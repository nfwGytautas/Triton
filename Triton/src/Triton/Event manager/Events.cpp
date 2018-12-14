#include "TRpch.h"
#include "Events.h"
#include "EventManager.h"

Triton::EventListener::EventListener()
{ 
	Core::EventManager::AddListener(this);
}

void Triton::EventListener::Listen(const EventType& event, EventBehavior& behavior)
{
	m_ListeningTo[(size_t)event] = true;
	m_RegisteredEvents[(size_t)event] = behavior;
}

void Triton::EventListener::StopListening(const EventType& event)
{
	m_ListeningTo[(size_t)event] = false;
}

void Triton::EventListener::React(Event& event)
{
	if (m_ListeningTo[(size_t)event.GetEventType()])
	{
		event.m_Handled = m_RegisteredEvents[(size_t)event.GetEventType()](event);
	}
}