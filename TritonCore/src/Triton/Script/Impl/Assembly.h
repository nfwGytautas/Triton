#pragma once

// Header file containing all the needed functions for Triton
// to scour the assembly of scripts

#include "Triton/Utility/Algorithm.h"

namespace Triton
{
	namespace Script
	{
		namespace Assembly
		{
			const char* c_TritonNamespace = "TritonEngine";

			// Loads an assembly object
			MonoAssembly* loadAssembly(MonoDomain* domain, const char* assembly)
			{
				MonoAssembly* assemblyObj = mono_domain_assembly_open(domain, assembly);

				if (!assemblyObj)
				{
					TR_SYSTEM_ERROR("Script engine could not open '{0}' assembly ('failed mono_domain_assembly_open')", Data::File::fileNameFromPath(assembly));
					return nullptr;
				}

				return assemblyObj;
			}

			// Gets an image from an assembly
			MonoImage* getImage(MonoAssembly* assembly)
			{
				MonoImage* image = mono_assembly_get_image(assembly);
				if (!image)
				{
					TR_SYSTEM_ERROR("Script engine could not get image from assembly ('failed mono_assembly_get_image')");
					return nullptr;
				}

				return image;
			}

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

			// Gets all classes who are inside TritonEngine name space
			std::vector<MonoClass*> getAllTritonClassesFromImage(MonoImage* image)
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

					auto name_spaces = Triton::Utility::ssplit(name_space, '.');

					if (name_spaces.size() == 0 || name_spaces[0] != c_TritonNamespace)
					{
						continue;
					}

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