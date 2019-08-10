#pragma once

// Header file containing all the needed functions for Triton
// to scour the assembly of scripts

namespace Triton
{
	namespace Script
	{
		namespace Assembly
		{
			// Search for all classes in an image
			std::vector<MonoClass*> getAllClassesFromImage(MonoImage* image)
			{
				std::vector<MonoClass*> foundClasses;

				const MonoTableInfo* table_info = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);

				int rows = mono_table_info_get_rows(table_info);

				/* For each row, get some of its values */
				for (int i = 0; i < rows; i++)
				{
					MonoClass* _class = nullptr;
					uint32_t cols[MONO_TYPEDEF_SIZE];
					mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
					const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
					const char* name_space = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
					_class = mono_class_from_name(image, name_space, name);

					foundClasses.push_back(_class);
				}

				return foundClasses;
			}

			// Get all classes that inherit ObjectScript
			std::vector<MonoClass*> getAllBehaviorClassesFromList(MonoClass* behaviourClass, std::vector<MonoClass*> classList)
			{
				std::vector<MonoClass*> foundClasses;

				for (auto klass : classList)
				{
					if (mono_class_is_subclass_of(klass, behaviourClass, true))
					{
						foundClasses.push_back(klass);
					}
				}

				return foundClasses;
			}
		}
	}
}