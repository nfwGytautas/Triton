#include "TRpch.h"
#include "File.h"

std::string Triton::Data::File::ReadAllText(std::string& aPath)
{
	std::ifstream is(aPath);
	std::stringstream buffer;
	buffer << is.rdbuf();
	return buffer.str();
}
