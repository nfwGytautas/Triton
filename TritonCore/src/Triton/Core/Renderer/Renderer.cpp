#include "TRpch.h"
#include "Renderer.h"

#include <glad\glad.h>
#include "Triton\Logger\Log.h"

Triton::Core::Renderer::Renderer()
{
	m_State = Singleton::State::GetInstance();
}

void Triton::Core::Renderer::Render()
{
	while (!m_Actions.empty()) {
		m_Actions.front()->Execute(m_State);
		m_Actions.pop();
	}
}