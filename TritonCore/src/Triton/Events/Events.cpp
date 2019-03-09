#include "TRpch.h"
#include "Events.h"
#include "EventManager.h"

Triton::EventListener::EventListener(Core::EventManager* mManager)
{ 
	m_ID = mManager->AddListener(*this);
}

void Triton::EventListener::React(Event& event)
{
	if (m_ListeningTo[static_cast<std::size_t>(event.GetEventType())])
	{
		event.m_Handled = m_RegisteredEvents[(size_t)event.GetEventType()](event);
	}
}