#pragma once

#include <string>

#include "TritonPlatform/CrossTypes/Shader.h"

namespace Triton
{
	namespace Impl
	{
		std::wstring s2ws(const std::string& s);
		DXGI_FORMAT sdtToDXGIFormat(Triton::PType::ShaderDataType type);
	}
}