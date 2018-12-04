#include <Triton.h>
#include <string>

#include "Triton\Event manager\ApplicationEvent.h"
#include "Triton\Event manager\KeyEvent.h"

bool windowResize(const Triton::Event& event)
{
	const Triton::WindowResizeEvent& wre = static_cast<const Triton::WindowResizeEvent&>(event);
	TR_TRACE("INSIDE RESIZE {0}", wre.ToString());
	return false;
}

bool keyPressed(const Triton::Event& event)
{
	const Triton::KeyPressedEvent& kpe = static_cast<const Triton::KeyPressedEvent&>(event);
	TR_TRACE("INSIDE KEY PRESS {0}", kpe.ToString());
	return false;
}

class Sandbox : public Triton::Application , private Triton::EventListener
{
public:
	Sandbox()
	{
		this->Listen(Triton::EventType::WindowResize, Triton::EventBehavior(windowResize));
		this->Listen(Triton::EventType::KeyPressed, Triton::EventBehavior(keyPressed));
	}

	~Sandbox()
	{

	}
};

Triton::Application* Triton::CreateApplication()
{
	return new Sandbox();
}