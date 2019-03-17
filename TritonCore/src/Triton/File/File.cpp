#include "TRpch.h"
#include "File.h"

std::string Triton::Data::File::ReadAllText(std::string& aPath)
{
	std::ifstream is(aPath);
	std::stringstream buffer;
	buffer << is.rdbuf();
	return buffer.str();
}

bool Triton::Data::File::Valid(std::string& aPath)
{
	struct stat buffer;
	return (stat(aPath.c_str(), &buffer) == 0);
}
