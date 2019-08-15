#include "TRpch.h"
#include "ScriptEngine.h"

#include "Triton/TRMacros.h"

// Mono libs
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>


#ifdef TR_DEBUG
#define TR_MONO_ROOT ../vendor/mono/
#define TR_CSHARP_ENGINE_PATH ../bin/TritonEngine.dll
#else
#define TR_MONO_ROOT mono/
#define TR_CSHARP_ENGINE_PATH mono/
#endif

#define M_CONC(A, B) M_CONC_(A, B)
#define M_CONC_(A, B) A##B

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#define TR_MONO_LIB STRINGIFY(M_CONC(TR_MONO_ROOT, lib))
#define TR_MONO_ETC STRINGIFY(M_CONC(TR_MONO_ROOT, etc))

#include <sstream>

#include "Triton/Logger/Log.h"
#include "Triton/File/File.h"
#include "Triton/Utility/Utility.h"

#include "Impl/ClassDefs.h"
#include "Impl/Enum.h"
#include "Impl/Interop.h"
#include "Impl/Assembly.h"
#include "Impl/Object.h"
#include "Impl/TritonEngine.h"

namespace Triton
{
	namespace Script
	{
		// For mono JIT
		MonoDomain* s_domain;

		class MethodParams::MethodParamsImpl
		{
		public:
			// Add string
			void addString(const std::string& str)
			{
				MonoString* pA = mono_string_new(s_domain, str.c_str());
				args.push_back((void*)pA);
			}

			// Add string array
			void addString_arr(const std::vector<std::string>& str)
			{
				MonoArray* pArray = mono_array_new(s_domain, mono_get_string_class(), str.size());

				for (uint32_t i = 0; i < str.size(); ++i)
				{
					MonoString* obj = mono_string_new(s_domain, str[i].c_str());
					mono_array_set(pArray, MonoString*, i, obj);
				}

				args.push_back((void*)pArray);
			}

			// The computed arguments
			std::vector<void*> args;
		};

		MethodParams::MethodParams()
		{
			m_impl = new MethodParamsImpl();
		}

		MethodParams::~MethodParams()
		{
			delete m_impl;
		}

		void Triton::Script::MethodParams::addStringParam(std::string str)
		{
			m_impl->addString(str);
		}

		void MethodParams::addStringParam_arr(std::vector<std::string> strArr)
		{
			m_impl->addString_arr(strArr);
		}

		std::vector<void*>& MethodParams::toArgs() const
		{
			return m_impl->args;
		}

		class DynamicSharpScript::DynamicScriptImpl
		{
			const char* c_UpdateMethod = "ObjectScript:OnUpdate(single)";
		public:
			DynamicScriptImpl(MonoClass* baseClass, MonoClass* klass, MonoObject* object)
				: m_klass(klass), m_object(object)
			{
				getMethods(baseClass);
			}

			// Invokes a method
			void* invoke(const std::string& methodName, std::vector<void*>& args)
			{
				if (m_methods.find(methodName) == m_methods.end())
				{
					TR_ERROR("Method with name: {0} doesn't exist", methodName);
					return nullptr;
				}

				MonoObject* res = mono_runtime_invoke(m_methods[methodName], m_object, args.data(), nullptr);
				if (mono_class_is_valuetype(mono_object_get_class(res)))
				{
					return mono_object_unbox(res);
				}
				else
				{
					return (void*)res;
				}
			}
		private:
			// Get the methods for script
			void getMethods(MonoClass* baseClass)
			{
				Object::getAllClassMethods(m_klass, m_methods);
			}
		private:
			// The class definition
			MonoClass* m_klass;
			// Instance of the ObjectScript object
			MonoObject* m_object;

			std::unordered_map<std::string, MonoMethod*> m_methods;
		};


		class SharpScript::ScriptImpl
		{
			const char* c_UpdateMethod = "ObjectScript:OnUpdate(single)";
		public:
			ScriptImpl(MonoClass* baseClass, MonoClass* klass, MonoObject* object, relay_ptr<GameObject> gObject)
				: m_klass(klass), m_object(object), m_gObject(gObject)
			{
				getMethods(baseClass);
				attachObject();
			}

			// Calls update method for ObjectScript
			void onUpdate(float delta)
			{
				//Set the arguments for the method
				void* args[1];
				args[0] = &delta;

				mono_runtime_invoke(m_method_update, m_object, args, nullptr);
			}

			// Check if underlying GameObject relay_ptr is valid
			bool isValid() const
			{
				return m_gObject.valid();
			}

			// Check if the script is attached to the specified object
			bool attachedTo(relay_ptr<GameObject> object) const
			{
				return m_gObject == object;
			}

			// Get the name of the script class
			std::string className()
			{
				return mono_class_get_name(m_klass);
			}

			// Attach the specified game object to the object
			void attachObject()
			{
				auto native_handle_field = mono_class_get_field_from_name(m_klass, ScriptClass::c_GameObjectNativeHandle);
				void* native_handle_value = &m_gObject;
				mono_field_set_value(m_object, native_handle_field, &native_handle_value);
			}

		private:
			// Get the methods for script
			void getMethods(MonoClass* baseClass)
			{
				m_method_update = Object::getMethod(c_UpdateMethod, m_klass);
			}
		private:
			// Game object relay_ptr
			relay_ptr<GameObject> m_gObject;

			// The class definition
			MonoClass* m_klass;
			// Instance of the ObjectScript object
			MonoObject* m_object;
			
			// Update method pointer
			MonoMethod* m_method_update;
		};


		class ScriptEngine::Impl
		{
		private:
			const char* c_TritonEnginePath = "";
		public:
			// Initializes the script engine
			bool setup()
			{
				m_engineInstance = new TritonEngine();

				// Set mono directories
				mono_set_dirs(
					TR_MONO_LIB,
					TR_MONO_ETC);

				// Create domain
				s_domain = mono_jit_init("MonoScript");
				if (!s_domain)
				{
					TR_SYSTEM_ERROR("Script engine could not create mono domain ('failed mono_jit_init')");
					return false;
				}

				// Inject all internal calls, all of them can be found in Interop.cs file
				injectInternalCalls();

				// Setup the engine instance
				if (!m_engineInstance->setup(s_domain))
				{
					TR_SYSTEM_ERROR("Script engine failed to setup the 'TritonEngine' instance");
					return false;
				}

				m_engineInstance->getAllEngineObjects(m_tClassMap);

				TR_SYSTEM_INFO("Script engine online");
				return true;
			}

			// Creates a new SharpScript objects from specified assembly file
			std::vector<SharpScriptLayout> loadAssembly(const char* assembly)
			{
				// The result vector
				std::vector<SharpScriptLayout> scripts;

				// Load the assembly
				MonoAssembly* assemb = Assembly::loadAssembly(s_domain, assembly);
				if (!assemb)
				{
					return scripts;
				}
				m_loadedAssemblies[assembly] = assemb;

				// Load the image
				MonoImage* image = Assembly::getImage(assemb);
				if (!image)
				{
					return scripts;
				}
				m_loadedImages[assembly] = image;

				// Get all classes in the image
				std::vector<MonoClass*> classes = Assembly::getAllClassesFromImage(image);
				if (classes.size() == 0)
				{
					return scripts;
				}

				// Handle all the class that have been found
				std::string nAssemb = mono_assembly_name_get_name(mono_assembly_get_name(assemb));

				auto objectScript = m_tClassMap[ScriptClass::c_ObjectScript];
				for (MonoClass* klass : classes)
				{
					std::string nSpace = mono_class_get_namespace(klass);
					std::string name = mono_class_get_name(klass);

					std::stringstream ss;
					ss << nAssemb << ":" << nSpace << ":" << name;

					m_allClasses[ss.str()] = klass;

					if (Object::isBaseOf(objectScript, klass))
					{
						m_behaviourClasses[ss.str()] = klass;
					}

					scripts.push_back({ nAssemb, nSpace, name});
				}

				return scripts;
			}

			// Creates an instance of SharpScript from the name of the class and the game object
			reference<SharpScript> createScriptInstance(const char* className, relay_ptr<GameObject> object)
			{
				if (m_behaviourClasses.find(className) == m_behaviourClasses.end())
				{
					TR_WARN("Class with name: {0}, doesn't exist make sure you included the assembly and the namespace", className);
					return nullptr;
				}

				auto klass = m_behaviourClasses[className];
				SharpScript* script = new SharpScript(
					new SharpScript::ScriptImpl(
						m_tClassMap[ScriptClass::c_ObjectScript], 
						klass, 
						Object::createObject(s_domain, klass),
						object));
				return script;
			}

			// Creates an instance of DynamicSharpScript from the name of the class
			reference<DynamicSharpScript> createDynamicScriptInstance(const char* className)
			{
				if (m_allClasses.find(className) == m_allClasses.end())
				{
					TR_WARN("Class with name: {0}, doesn't exist make sure you included the assembly and the namespace", className);
					return nullptr;
				}

				auto klass = m_allClasses[className];
				DynamicSharpScript* script = new DynamicSharpScript(
					new DynamicSharpScript::DynamicScriptImpl(
						m_tClassMap[ScriptClass::c_ObjectScript],
						klass,
						Object::createObject(s_domain, klass)));

				return script;
			}

			// Gets the engine relay_ptr
			relay_ptr<TritonEngine> engine()
			{
				return m_engineInstance;
			}

			// Validates all the maps
			// Checks if any entries point to NULL and deletes them
			void validate()
			{
				Utility::map_remove_of<std::string, MonoClass*>(m_behaviourClasses, [](MonoClass* ptr) -> bool {return (ptr == nullptr || ptr == NULL); });
				Utility::map_remove_of<std::string, MonoClass*>(m_allClasses, [](MonoClass* ptr) -> bool {return (ptr == nullptr || ptr == NULL); });
				Utility::map_remove_of<std::string, MonoClass*>(m_tClassMap, [](MonoClass* ptr) -> bool {return (ptr == nullptr || ptr == NULL); });

				Utility::map_remove_of<std::string, MonoAssembly*>(m_loadedAssemblies, [](MonoAssembly* ptr) -> bool {return (ptr == nullptr || ptr == NULL); });

				Utility::map_remove_of<std::string, MonoImage*>(m_loadedImages, [](MonoImage* ptr) -> bool {return (ptr == nullptr || ptr == NULL); });
			}
		private:
			// Adds internal calls
			void injectInternalCalls()
			{
				Interop::addInternals();
			}
		private:
			// Pointer to the instance of the engine
			TritonEngine* m_engineInstance;

			// Map of all behavior classes
			// Assembly:Namespace:ClassName
			std::unordered_map<std::string, MonoClass*> m_behaviourClasses;

			// Map of all classes
			// Assembly:Namespace:ClassName
			std::unordered_map<std::string, MonoClass*> m_allClasses;

			// A map of assembly path and object
			std::unordered_map<std::string, MonoAssembly*> m_loadedAssemblies;
			// A map of images from loaded from assembly paths
			std::unordered_map<std::string, MonoImage*> m_loadedImages;
			
			// Map of Triton class name to MonoClass pointer
			std::unordered_map<std::string, MonoClass*> m_tClassMap;
		};


		ScriptEngine::ScriptEngine()
		{
			m_impl = new Impl();
		}

		ScriptEngine::~ScriptEngine()
		{
			delete m_impl;
		}

		std::vector<SharpScriptLayout> ScriptEngine::loadAssembly(std::string assemblyPath)
		{
			auto layouts = m_impl->loadAssembly(assemblyPath.c_str());

			for (auto& layout : layouts)
			{
				m_scriptLayouts.push_back(layout);
			}

			return layouts;
		}

		reference<DynamicSharpScript> ScriptEngine::getDynamicScript(const std::string & className)
		{
			return m_impl->createDynamicScriptInstance(className.c_str());
		}

		void ScriptEngine::attachScript(std::string className, relay_ptr<GameObject> object)
		{
			m_sharpScripts.push_back(m_impl->createScriptInstance(className.c_str(), object));
		}

		void ScriptEngine::detachScript(std::string className, relay_ptr<GameObject> object)
		{
			auto new_end = std::remove_if(m_sharpScripts.begin(), m_sharpScripts.end(),
				[&](const reference<SharpScript>& script)
			{ 
				if (script->getName() == className)
				{
					if (script->attachedTo(object))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			});

			m_sharpScripts.erase(new_end, m_sharpScripts.end());
		}

		void Triton::Script::ScriptEngine::onMessage(size_t message, void* payload)
		{
			switch (message)
			{
			case (size_t)Core::TritonMessageType::ClassRegistered:
			{
				m_impl->setup();
				return;
			}
			case (size_t)Core::TritonMessageType::Update:
			{
				validateScripts();
				updateScripts();
				return;
			}
			}
		}

		void ScriptEngine::updateScripts()
		{
			auto timer = getClassByID((size_t)Core::TritonClasses::Timer).as<Utility::Timer>();

			for (auto script : m_sharpScripts)
			{
				script->onUpdate(timer->updateDelta());
			}
		}

		void ScriptEngine::validateScripts()
		{
			auto new_end = std::remove_if(m_sharpScripts.begin(), m_sharpScripts.end(),
				[&](const reference<SharpScript>& script)
			{
				if(script.valid())
				{
					return !script->isValid();
				}
				else
				{
					return true;
				}
			});

			m_sharpScripts.erase(new_end, m_sharpScripts.end());
		}

		SharpScript::SharpScript(ScriptImpl* impl)
			: m_impl(impl)
		{
		}

		SharpScript::~SharpScript()
		{
			delete m_impl;
		}

		std::string SharpScript::getName() const
		{
			return m_impl->className();
		}

		bool SharpScript::isValid() const
		{
			return m_impl->isValid();
		}

		void SharpScript::onUpdate(float delta)
		{
			m_impl->onUpdate(delta);
		}

		bool SharpScript::attachedTo(relay_ptr<GameObject> object) const
		{
			return m_impl->attachedTo(object);
		}

		Triton::Script::DynamicSharpScript::DynamicSharpScript(DynamicScriptImpl* impl)
			: m_impl(impl)
		{
		}

		Triton::Script::DynamicSharpScript::~DynamicSharpScript()
		{
			delete m_impl;
		}

		void* DynamicSharpScript::invokeMethod(const std::string& methodName, std::vector<void*>& args)
		{
			return m_impl->invoke(methodName, args);
		}
}
}