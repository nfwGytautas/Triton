#include "TRpch.h"
#include "EventManager.h"

std::vector<Triton::Event*> Triton::Core::EventManager::m_EventBuffer;
std::vector<Triton::EventListener*> Triton::Core::EventManager::m_Listeners;

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
			else if(m_EventBuffer[i]->IsHandled())
			{
				delete m_EventBuffer[i];
				m_EventBuffer.erase(m_EventBuffer.begin() + i);
				break;
			}
		}
	}
}

void Triton::Core::EventManager::AddListener(EventListener* listener)
{
	m_Listeners.push_back(listener);
}

void Triton::Core::EventManager::ClearBuffer()
{
	for (unsigned int i = 0; i < m_EventBuffer.size(); i++)
	{
		delete m_EventBuffer[i];
	}
	m_EventBuffer.clear();
}
