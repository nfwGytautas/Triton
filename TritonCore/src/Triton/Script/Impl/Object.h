#pragma once

// Header file containing helper function for easier MonoObject, MonoClass objects

namespace Triton
{
	namespace Script
	{
		namespace Object
		{
			// Creates a MonoObject from class object
			MonoObject* createObject(MonoDomain* domain, MonoClass* klass)
			{
				//Create a instance of the class
				MonoObject* object = mono_object_new(domain, klass);
				if (!object)
				{
					TR_SYSTEM_ERROR("Script engine could not create class instance ('failed mono_object_new')");
					return nullptr;
				}

				mono_runtime_object_init(object);

				return object;
			}

			// Gets the specified 'method' from 'klass' object
			// returns nullptr if failed
			MonoMethod* getMethod(const char* method, MonoClass* klass)
			{
				//Build a method description object
				MonoMethodDesc* methodDesc;
				methodDesc = mono_method_desc_new(method, NULL);
				if (!methodDesc)
				{
					TR_SYSTEM_ERROR("Script engine could not get method description for '{0}' ('failed mono_method_desc_new')", method);
					return nullptr;
				}

				//Search the method in the class
				MonoMethod* _method = mono_method_desc_search_in_class(methodDesc, klass);
				if (!_method)
				{
					TR_SYSTEM_ERROR("Script engine could not find method '{0}' ('failed mono_method_desc_search_in_class')", method);
					return nullptr;
				}

				mono_method_desc_free(methodDesc);

				return _method;
			}

			// Helper function for printing out all methods inside a class
			void getAllClassMethods(MonoClass* klass)
			{
				void* iter = NULL;
				MonoMethod* method;
				while (method = mono_class_get_methods(klass, &iter))
				{
					TR_SYSTEM_TRACE("Method: {0}", mono_method_full_name(method, 1));
				}
			}

			// Fills the specified map with all methods inside the class
			void getAllClassMethods(MonoClass* klass, std::unordered_map<std::string, MonoMethod*>& methods)
			{
				void* iter = NULL;
				MonoMethod* method;
				while (method = mono_class_get_methods(klass, &iter))
				{
					methods[mono_method_get_name(method)] = method;
				}
			}

			// Helper method for printing out all interfaces of a class
			void getAllInterfaces(MonoClass* klass)
			{
				void* iter = NULL;
				MonoClass* klc;
				while (klc = mono_class_get_interfaces(klass, &iter))
				{
					TR_SYSTEM_TRACE("Class: {0}", mono_class_get_name(klc));
				}
			}

			// Checks if the object is a subclass of another object
			bool isBaseOf(MonoClass* base, MonoClass* other)
			{
				return mono_class_is_subclass_of(other, base, NULL);
			}
		}
	}
}