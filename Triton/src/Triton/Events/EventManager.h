#pragma once
#include "Triton\TRMacros.h"
#include "Events.h"

namespace Triton
{
	namespace Core
	{
		class TRITON_API EventManager
		{
		public:
			static void Post(Event* event);
			static void Dispatch();
			static size_t AddListener(EventListener& listener);
			static void RemoveListener(EventListener& listener);
			static void ClearBuffer();
		private:
			static size_t m_NextListenerID;
			static std::vector<Event*> m_EventBuffer;
			static std::vector<EventListener*> m_Listeners;
		};
	}
}