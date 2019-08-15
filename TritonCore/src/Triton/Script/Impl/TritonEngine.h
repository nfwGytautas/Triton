#pragma once

// Implementation of the TritonEngine that is used in C#

namespace Triton
{
	namespace Script
	{
		// The main class that powers Triton scripting capabilities
		// It contains everything one needs to run C# scripts for Triton
		class TritonEngine
		{
		private:
			const char* c_setupMethodDescStr = "TritonEngine:Setup()";
		public:
			TritonEngine()
			{}
			virtual ~TritonEngine() 
			{}

			// Sets up the C# side of TritonEngine
			bool setup(MonoDomain* domain)
			{
				// Open the engine assembly
				const char* assemblyPath = STRINGIFY(TR_CSHARP_ENGINE_PATH);
				m_engineAssembly = mono_domain_assembly_open(domain, assemblyPath);
				if (!m_engineAssembly)
				{
					TR_SYSTEM_ERROR("Script engine could not open 'TritonEngine' assembly ('failed mono_domain_assembly_open')");
					return false;
				}

				//Get a image from the assembly
				m_engineImage = mono_assembly_get_image(m_engineAssembly);
				if (!m_engineImage)
				{
					TR_SYSTEM_ERROR("Script engine could not get 'TritonEngine' image from assembly ('failed mono_assembly_get_image')");
					return false;
				}

				//Get the class
				m_class = mono_class_from_name(m_engineImage, "TritonEngine", "TritonEngine");
				if (!m_class)
				{
					TR_SYSTEM_ERROR("Script engine could not get 'TritonEngine' class ('failed mono_class_from_name')");
					return false;
				}

				//Create a instance of the class
				m_instance = mono_object_new(domain, m_class);
				if (!m_instance)
				{
					TR_SYSTEM_ERROR("Script engine could not create 'TritonEngine' instance ('failed mono_object_new')");
					return false;
				}

				//Call its default constructor
				TR_SYSTEM_TRACE("Initializing 'TritonEngine'");
				mono_runtime_object_init(m_instance);

				// Get all class methods
				getMethods();

				// C# setup
				mono_runtime_invoke(m_method_setup, m_instance, nullptr, nullptr);

				return true;
			}
			
			// Gets the 'ObjectScript' class definition from engine image
			MonoClass* getObjectScriptDef()
			{
				MonoClass* ptrToSet = mono_class_from_name(m_engineImage, "TritonEngine", "ObjectScript");
				if (!ptrToSet)
				{
					TR_SYSTEM_ERROR("Script engine could not get 'ObjectScript' class ('failed mono_class_from_name')");
					return nullptr;
				}

				return ptrToSet;
			}

			// Gets the 'ObjectScript' class definition from engine image
			MonoClass* getEngineObjectDef(std::string objectName, std::string nspace = "TritonEngine")
			{
				MonoClass* ptrToSet = mono_class_from_name(m_engineImage, nspace.c_str(), objectName.c_str());
				if (!ptrToSet)
				{
					TR_SYSTEM_ERROR("Script engine could not get '{0}' class ('failed mono_class_from_name')", objectName);
					return nullptr;
				}

				return ptrToSet;
			}

			void getAllEngineObjects(std::unordered_map<std::string, MonoClass*>& map)
			{
				for (auto klass : Assembly::getAllTritonClassesFromImage(m_engineImage))
				{
					map[mono_class_get_name(klass)] = klass;
				}
			}
		private:
			void getMethods()
			{
				TR_SYSTEM_TRACE("Acquiring class methods");

				m_method_setup = Object::getMethod(c_setupMethodDescStr, m_class);

				TR_SYSTEM_INFO("Methods for 'TritonEngine' acquired");
			}
		private:
			// Assembly
			MonoAssembly* m_engineAssembly;
			// Image
			MonoImage* m_engineImage;

			// Instance
			MonoClass* m_class;
			MonoObject* m_instance;

			// MonoMethods
			MonoMethod* m_method_setup;
		};
	}
}