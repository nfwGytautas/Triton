#pragma once

#include "Triton\TRMacros.h"
#include "Triton\Core\Display.h"

#include <GLFW\glfw3.h>

namespace Triton
{
	namespace Core
	{
		class TRITON_API WindowsDisplay : public Display
		{
		public:
			WindowsDisplay(const DisplaySettings& aSettings);
			virtual ~WindowsDisplay();

			// Inherited via Display
			inline unsigned int GetWidth() const override;
			inline unsigned int GetHeight() const override;

			bool Closed() override;
			void OnUpdate() override;

			void SetVSync(bool aState) override;
			bool IsVSync() override;

			void ShowCursor(bool aState) override;

		protected:
			virtual void Init(const DisplaySettings& aSettings);
			virtual void ShutDown();

		private:
			void SetUpCallbacks();

			virtual void SetEventReceiver(EventReceiver* aReceiver) override;
		private:
			GLFWwindow* m_Window;

			struct WindowData
			{
				std::string Title;
				unsigned int Width, Height;
				bool VSync;

				EventReceiver* Receiver;
			};

			WindowData m_Data;
		};

	}
}