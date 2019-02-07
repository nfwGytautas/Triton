#include "TRpch.h"
#include "EventManager.h"

#include "Events.h"

std::vector<Triton::Event*> Triton::Core::EventManager::m_EventBuffer;
std::vector<Triton::EventListener*> Triton::Core::EventManager::m_Listeners;
size_t Triton::Core::EventManager::m_NextListenerID = 0;

void Triton::Core::EventManager::Post(Event* event)
{
	m_EventBuffer.push_back(event);
}

void Triton::Core::EventManager::Dispatch()
{
	for (unsigned int i = 0; i < m_EventBuffer.size(); i++)
	{
		for (EventListener* listener : m_Listeners)
		{
			if (!m_EventBuffer[i]->IsHandled())
			{
				listener->React(*m_EventBuffer[i]);
			}
			else
			{
				delete m_EventBuffer[i];
				m_EventBuffer.erase(m_EventBuffer.begin() + i);
				i--;
				break;
			}
		}
	}
}

size_t Triton::Core::EventManager::AddListener(EventListener& listener)
{
	m_Listeners.push_back(&listener);
	return ++m_NextListenerID;
}

void Triton::Core::EventManager::RemoveListener(EventListener& listener)
{
	m_Listeners.erase(std::remove(m_Listeners.begin(), m_Listeners.end(), &listener), m_Listeners.end());
}

void Triton::Core::EventManager::ClearBuffer()
{
	for (auto item : m_EventBuffer)
	{
		delete item;
	}
	m_EventBuffer.clear();
}
