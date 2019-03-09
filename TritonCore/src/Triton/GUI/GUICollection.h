#pragma once

#include "GUI.h"
#include "Triton\Events\Events.h"

#include "Triton\Events\KeyEvent.h"
#include "Triton\Events\MouseEvent.h"
#include "Triton\Events\ApplicationEvent.h"


namespace Triton
{
	namespace UI
	{

		class TRITON_API GUICollection : protected EventListener
		{
		public:
			GUICollection(Core::EventManager* mManager);
			GUICollection(Core::EventManager* mManager, size_t aSize);
			~GUICollection();

			static void InitGUI(unsigned int aWidth, unsigned int aHeight);

			void UpdateCollection(float aDelta);
			void DrawCollection();

			void AddGUI(std::shared_ptr<GUI> aGUI);

		protected:
			bool OnMouseButtonPressedEvent(const Triton::Event& e);
			bool OnMouseButtonReleasedEvent(const Triton::Event& e);
			bool OnMouseMovedEvent(const Triton::Event& e);
			bool OnMouseScrolledEvent(const Triton::Event& e);
			bool OnKeyPressedEvent(const Triton::Event& e);
			bool OnKeyReleasedEvent(const Triton::Event& e);
			bool OnKeyInputEvent(const Triton::Event& e);
			bool OnWindowResizeEvent(const Triton::Event& e);
			
		private:
			std::vector<std::shared_ptr<GUI>> mGUIS;
		};

	}
}