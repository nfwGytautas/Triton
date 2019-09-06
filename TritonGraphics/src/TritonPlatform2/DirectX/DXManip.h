#pragma once

#include <string>

#include "TritonPlatform2/CrossTypes/Shader.h"

namespace Triton
{
	namespace Impl
	{
		std::wstring s2ws(const std::string& s);
		DXGI_FORMAT sdtToDXGIFormat(Triton::Graphics::ShaderDataType type);
	}
}