#pragma once
#include "TritonPlatform/CrossTypes/Base.h"

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
			void* hwnd;

			// The device that is currently used by the engine
			void* device;

			// The device context
			void* deviceContext;
		};
	}
}