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
				m_domain = mono_jit_init("MonoScript");
				if (!m_domain)
				{
					TR_SYSTEM_ERROR("Script engine could not create mono domain ('failed mono_jit_init')");
					return false;
				}

				// Inject all internal calls, all of them can be found in Interop.cs file
				injectInternalCalls();

				// Setup the engine instance
				if (!m_engineInstance->setup(m_domain))
				{
					TR_SYSTEM_ERROR("Script engine failed to setup the 'TritonEngine' instance");
					return false;
				}

				m_engineInstance->getAllEngineObjects(m_tClassMap);

				TR_SYSTEM_INFO("Script engine online");
				return true;
			}

			// Creates a new SharpScript objects from specified assembly file
			std::vector<SharpScriptLayout> scriptsFromAssembly(const char* assembly)
			{
				// The result vector
				std::vector<SharpScriptLayout> scripts;

				// Load the assembly
				MonoAssembly* assemb = Assembly::loadAssembly(m_domain, assembly);
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

				// Filter out the classes that are not scripts
				std::vector<MonoClass*> behaviours = Assembly::getAllBehaviorClassesFromList(m_tClassMap[ScriptClass::c_ObjectScript], classes);
				if (behaviours.size() == 0)
				{
					return scripts;
				}

				// Add the classes to the list
				for (MonoClass* klass : behaviours)
				{
					m_loadedClasses.push_back(klass);
					m_classMap[mono_class_get_name(klass)] = klass;
					scripts.push_back({ mono_class_get_name(klass), assembly });
				}

				return scripts;
			}

			// Creates an instance of SharpScript from the name of the class and the game object
			reference<SharpScript> createScriptInstance(const char* className, relay_ptr<GameObject> object)
			{
				auto klass = m_classMap[className];
				SharpScript* script = new SharpScript(
					new SharpScript::ScriptImpl(
						m_tClassMap[ScriptClass::c_ObjectScript], 
						klass, 
						Object::createObject(m_domain, klass), 
						object));
				return script;
			}
		private:
			// Adds internal calls
			void injectInternalCalls()
			{
				Interop::addInternals();
			}
		private:
			// For mono JIT
			MonoDomain* m_domain;

			// Pointer to the instance of the engine
			TritonEngine* m_engineInstance;

			// A map of assembly path and object
			std::unordered_map<std::string, MonoAssembly*> m_loadedAssemblies;
			// A map of images from loaded from assembly paths
			std::unordered_map<std::string, MonoImage*> m_loadedImages;

			// A vector of all loaded MonoClass pointers
			std::vector<MonoClass*> m_loadedClasses;
			// Map of class name to MonoClass pointer
			std::unordered_map<std::string, MonoClass*> m_classMap;
			
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
			auto layouts = m_impl->scriptsFromAssembly(assemblyPath.c_str());

			for (auto& layout : layouts)
			{
				m_scriptLayouts.push_back(layout);
			}

			return layouts;
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
	}
}