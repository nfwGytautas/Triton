#include "TRpch.h"
#include "ShaderProgram.h"

Triton::Data::ShaderProgram::ShaderProgram(size_t assetID, reference<PType::Shader> object)
	: Asset(assetID), Program(object)
{
}
