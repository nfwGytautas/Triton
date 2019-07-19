#include <TRpch.h>
#include "RenderBuffer.h"

Triton::Core::RenderBuffer::RenderBuffer()
{
}

void Triton::Core::RenderBuffer::newFrame()
{
	m_context->renderer->newFrame(TR_CLEAR_COLOR);
}

void Triton::Core::RenderBuffer::clear()
{
	for (unsigned int i = 0; i < m_commands.size(); i++)
	{
		delete m_commands[i];
	}

	m_commands.clear();
}

void Triton::Core::RenderBuffer::exec()
{
	m_bufferState.Context = m_context;
	for (unsigned int i = 0; i < m_commands.size(); i++)
	{
		m_commands[i]->execute(m_bufferState);
	}

	m_context->renderer->endFrame();

	clear();
}

void Triton::Core::RenderBuffer::onMessage(size_t message, void * payload)
{
	switch (message)
	{
	case (size_t)TritonMessageType::ClassRegistered:
	{
		m_context = this->getClassByID((size_t)Core::TritonClasses::Context).as<PType::Context>();

		return;
	}
	}
}

void Triton::RCommands::Draw::execute(BufferState& bufferState)
{
	bufferState.Context->renderer->render(bufferState.VAOIndiceCount);
}
