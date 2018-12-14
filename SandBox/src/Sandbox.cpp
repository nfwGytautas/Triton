#include <Triton.h>
#include <string>

#include "Triton\Event manager\KeyEvent.h"

#include "Triton\Core\Shader\Shader.h"

bool keyPressed(const Triton::Event& event)
{
	const Triton::KeyPressedEvent& kpe = static_cast<const Triton::KeyPressedEvent&>(event);
	TR_TRACE("INSIDE KEY PRESS {0}", kpe.ToString());
	return true;
}

bool keyReleased(const Triton::Event& event)
{
	const Triton::KeyReleasedEvent& kpe = static_cast<const Triton::KeyReleasedEvent&>(event);
	TR_TRACE("INSIDE KEY RELEASE {0}", kpe.ToString());
	return true;
}

class Sandbox : public Triton::Application , private Triton::EventListener
{
public:
	Sandbox()
	{
		this->Listen(Triton::EventType::KeyPressed, Triton::EventBehavior(keyPressed));
		this->Listen(Triton::EventType::KeyReleased, Triton::EventBehavior(keyReleased));

		Triton::Core::Shader shader("D:/Programming/Test files/nfw/shaders/vertex.shader", "D:/Programming/Test files/nfw/shaders/fragment.shader");
	}

	~Sandbox()
	{

	}
};

Triton::Application* Triton::CreateApplication()
{
	return new Sandbox();
}