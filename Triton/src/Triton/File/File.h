#pragma once
#include "TRpch.h"
#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Data
	{
		struct ModelData
		{
			std::string testdata;
		};


		//Class for reading all types of external data
		class File
		{
		public:
			File();
			~File();

			bool ReadAllSources();

			template<class T>
			T& ExtractData()
			{

			}

			void AddSource(const char* Path);
			void AddSource(std::string& Path);
		private:
			std::vector<std::string>& m_FilePaths;
			std::string& m_Data;
		};
	}
}