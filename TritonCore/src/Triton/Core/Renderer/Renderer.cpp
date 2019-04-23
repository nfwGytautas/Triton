#include "TRpch.h"
#include "Renderer.h"

Triton::Core::Renderer::Renderer()
{
	m_State = std::make_shared<ContextState>();
}

void Triton::Core::Renderer::Render()
{
	this->Execute(*m_State.get());
}
