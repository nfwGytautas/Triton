#include "TRpch.h"
#include "Events.h"
#include "EventManager.h"

Triton::EventListener::EventListener()
{ 
	Core::EventManager::AddListener(this);
}

void Triton::EventListener::React(Event& event)
{
	if (m_ListeningTo[(size_t)event.GetEventType()])
	{
		event.m_Handled = m_RegisteredEvents[(size_t)event.GetEventType()](event);
	}
}