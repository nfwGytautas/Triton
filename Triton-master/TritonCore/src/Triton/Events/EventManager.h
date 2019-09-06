#pragma once
#include "Triton\TRMacros.h"
#include "Events.h"

namespace Triton
{
	namespace Core
	{
		class  EventManager
		{
		public:
			EventManager();

			void Post(Event* event);
			void Dispatch();
			size_t AddListener(EventListener& listener);
			void RemoveListener(EventListener& listener);
			void ClearBuffer();
		private:
			size_t m_NextListenerID;
			std::vector<Event*> m_EventBuffer;
			std::vector<EventListener*> m_Listeners;
		};
	}
}