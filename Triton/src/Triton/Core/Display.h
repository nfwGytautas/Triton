#pragma once

#include "Triton\TRMacros.h"

namespace Triton {
	namespace Core {
		
		//Class that handles the display functionality
		class TRITON_API Display
		{
		public:
			Display();
			~Display();

			bool Closed();
			void Update();
		};
	}
}