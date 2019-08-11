#pragma once

namespace Triton
{
	namespace Enum
	{
		enum VecComponent
		{
			X = 0,
			Y = 1,
			Z = 2
		};

		// Get value of enum
		int getEnumVal(MonoType* _enum)
		{
			return mono_type_get_type(_enum);
		}

		// Parse a class to an enum
		MonoType* objectToEnum(MonoClass* klass)
		{
			return mono_class_enum_basetype(klass);
		}

		// Check if class is an enum
		bool isEnum(MonoClass* klass)
		{
			return mono_class_is_enum(klass);
		}
	}
}