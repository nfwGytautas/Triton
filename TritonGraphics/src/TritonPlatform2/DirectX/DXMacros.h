#pragma once

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define DX_CONTEXT(x) static_cast<Triton::Graphics::DXContext*>(x)

#define OBJECT_AS(type, obj) dynamic_cast<type*>(obj);

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