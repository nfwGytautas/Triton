#pragma once

#include <iterator>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <functional>

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

		// Removes all entries from map that satisfy the predicate
		template<typename Key, typename Value>
		void map_remove_of(std::unordered_map<Key, Value>& map, std::function<bool(Value)> pred)
		{
			for (auto it = std::begin(map); it != std::end(map);)
			{
				if (pred(it->second))
				{
					it = map.erase(it);
				}
				else
				{
					++it;
				}
			}
		}


	}
}