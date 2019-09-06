#pragma once

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/Core/TritonClass.h"
#include "Triton/Core/Wrapers/Viewport.h"

namespace Triton
{
	class GameWindow : public Core::TritonClass
	{
	public:
		virtual void onMessage(size_t message, void* payload) override;

		unsigned int getWidth()
		{
			return m_gameViewport->Width;
		}
		unsigned int getHeight()
		{
			return m_gameViewport->Height;
		}
		void* getTexture()
		{
			return m_gameViewport->Framebuffer->getAsTexture();
		}
		reference<Data::Viewport> getViewport()
		{
			return m_gameViewport;
		}
	private:
		reference<Data::Viewport> m_gameViewport;
	};
}