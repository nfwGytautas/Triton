#pragma once
#include "TRpch.h"
#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Data
	{
		typedef unsigned char FileSourceFlag;

		enum class FileSourceFlags
		{
			Append = BIT(0),
			Seperate = BIT(1),
		};

		class File
		{
		public:
			File();
			~File();

			bool AddSource(const char* Path, FileSourceFlag Flags);
			bool AddSource(std::string& Path, FileSourceFlag Flags);
		private:
			std::vector<std::string>& m_Source;
			std::string m_Extension;
		};
	}
}