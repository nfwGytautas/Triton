#include <TRpch.h>
#include "RenderBuffer.h"

Triton::Core::RenderBuffer::RenderBuffer()
{
}

void Triton::Core::RenderBuffer::clear()
{
	for (unsigned int i = 0; i < m_commands.size(); i++)
	{
		delete m_commands[i];
	}

	m_commands.clear();
}

void Triton::Core::RenderBuffer::exec(PType::Context* context)
{
	m_bufferState.Context = context;
	for (unsigned int i = 0; i < m_commands.size(); i++)
	{
		m_commands[i]->execute(m_bufferState);
	}
}

void Triton::RCommands::Draw::execute(BufferState& bufferState)
{
	bufferState.Context->renderer->render(bufferState.VAOIndiceCount);
}
