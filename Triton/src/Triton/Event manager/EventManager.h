#pragma once
#include "Events.h"

namespace Triton
{
	namespace Core
	{
		class TRITON_API EventManager
		{
		public:
			static void Post(Event& event);
			static void Dispatch();
			static void AddListener(EventListener* listener);
		private:
			static std::vector<Event*> m_EventBuffer;
			static std::vector<EventListener*> m_Listeners;
		};
	}
}