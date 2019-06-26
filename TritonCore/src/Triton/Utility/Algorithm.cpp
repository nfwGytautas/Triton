#include "TRpch.h"
#include "Algorithm.h"
#include <fstream>

namespace Triton
{
	namespace Utility
	{

		std::vector<std::string> ssplit(const std::string& s, char delim)
		{
			std::vector<std::string> elems;
			Impl::tstringsplit(s, delim, std::back_inserter(elems));
			return elems;
		}

		std::string& rtrim(std::string& s, const char* t)
		{
			s.erase(s.find_last_not_of(t) + 1);
			return s;
		}

		std::string& ltrim(std::string& s, const char* t)
		{
			s.erase(0, s.find_first_not_of(t));
			return s;
		}

		std::string& trim(std::string& s, const char* t)
		{
			return ltrim(rtrim(s, t), t);
		}

	}
}