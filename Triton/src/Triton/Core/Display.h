#pragma once

#include "Triton\TRMacros.h"

namespace Triton {
	namespace Core {
		
		struct DisplaySettings
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;

			DisplaySettings(std::string aTitle = "Triton display", unsigned int aWidth = 600, unsigned int aHeight = 600)
				:Title(aTitle), Width(aWidth), Height(aHeight)
			{ }
		};

		//Class that handles the display functionality
		class TRITON_API Display
		{
		public:
			virtual ~Display() { };

			virtual bool Closed() = 0;
			virtual void OnUpdate() = 0;

			virtual unsigned int GetWidth() const = 0;
			virtual unsigned int GetHeight() const = 0;

			virtual void SetVSync(bool aState) = 0;
			virtual bool IsVSync() = 0;

			static Display* Create(const DisplaySettings& settings = DisplaySettings());
		};
	}
}