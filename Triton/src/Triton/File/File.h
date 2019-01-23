#pragma once
#include "TRpch.h"
#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Data
	{
		
		class TRITON_API File
		{
		public:
			static std::string ReadAllText(std::string& aPath);
		};

	}
}