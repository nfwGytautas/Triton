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
				if (!getMethods())
				{
					TR_SYSTEM_ERROR("Failed to get 'TritonEngine' methods");
					return false;
				}

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
		private:
			bool getMethods()
			{
				TR_SYSTEM_TRACE("Acquiring class methods");

				//Build a method description object
				MonoMethodDesc* setupMethodDesc;
				setupMethodDesc = mono_method_desc_new(c_setupMethodDescStr, NULL);
				if (!setupMethodDesc)
				{
					TR_SYSTEM_ERROR("Script engine could not get 'TritonEngine' method description for '{0}' ('failed mono_method_desc_new')", c_setupMethodDescStr);
					return false;
				}

				//Search the method in the image
				m_method_setup = mono_method_desc_search_in_image(setupMethodDesc, m_engineImage);
				if (!m_method_setup)
				{
					TR_SYSTEM_ERROR("Script engine could not find 'TritonEngine' method '{0}' ('failed mono_method_desc_search_in_image')", c_setupMethodDescStr);
					return false;
				}

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