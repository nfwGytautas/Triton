#pragma once

// Header file containing helper function for easier MonoObject, MonoClass objects

namespace Triton
{
	namespace Script
	{
		namespace Object
		{
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
		}
	}
}