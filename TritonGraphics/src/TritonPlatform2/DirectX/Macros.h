#pragma once

#include "Triton\Logger\Log.h"

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define DX_CONTEXT(x) static_cast<Triton::Graphics::DXContext*>(x)

#define OBJECT_AS(type, obj) dynamic_cast<type*>(obj);

#define PLATFORM_NAMESPACE_BEGIN namespace Triton { namespace Graphics {
#define PLATFORM_NAMESPACE_END } }

namespace Triton
{
	namespace Graphics
	{
		class DXContext;
		class DXFactory;
		class DXRenderer;
		class DXWindow;

		class DXShader;

		class DXTexture;
		class DXVAO;
		class DXBitmap;
		class DXFramebuffer;

		class DXPacket;
	}
}