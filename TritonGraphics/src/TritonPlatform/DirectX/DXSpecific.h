#pragma once
#include "TritonPlatform/CrossTypes/Base.h"

#include <Windows.h>
#include <d3d11.h>

namespace Triton
{
	namespace PType
	{
		// Platform specific packet that contains the data of the current platform context
		// Data modified in the packet gets modified for the whole engine
		class DXPacket : public PlatformObject
		{
		public:
			// The hwnd of Triton's created window
			HWND hwnd;

			// The device that is currently used by the engine
			ID3D11Device* device;

			// The device context
			ID3D11DeviceContext* deviceContext;
		};
	}
}