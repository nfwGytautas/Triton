#pragma once

#include <iterator>
#include <vector>
#include <sstream>

namespace Triton
{
	namespace Utility
	{
		namespace Impl
		{
			template<typename T>
			void tstringsplit(const std::string &s, char delim, T result) {
				std::stringstream ss(s);
				std::string item;
				while (std::getline(ss, item, delim)) {
					if (!item.empty())
					{
						*(result++) = item;
					}
				}
			}
		}

		// Split string by a delim character
		std::vector<std::string> ssplit(const std::string &s, char delim);

		// trim from end of string (right)
		std::string& rtrim(std::string& s, const char* t);

		// trim from beginning of string (left)
		std::string& ltrim(std::string& s, const char* t);
		
		// trim from both ends of string (right then left)
		std::string& trim(std::string& s, const char* t);

	}
}