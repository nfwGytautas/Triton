#include <Triton.h>

#include <string>

bool keyPressed(const Triton::Event& event)
{
	const Triton::KeyPressedEvent& kpe = dynamic_cast<const Triton::KeyPressedEvent&>(event);
	TR_TRACE("INSIDE KEY PRESS {0}", kpe.ToString());
	return true;
}

bool keyReleased(const Triton::Event& event)
{
	const Triton::KeyReleasedEvent& kpe = dynamic_cast<const Triton::KeyReleasedEvent&>(event);
	TR_TRACE("INSIDE KEY RELEASE {0}", kpe.ToString());
	return true;
}

class Sandbox : public Triton::Application , private Triton::EventListener
{
public:
	Sandbox()
	{
		this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(keyPressed));
		this->Listen<Triton::KeyReleasedEvent>(Triton::EventBehavior(keyReleased));
	}

	~Sandbox()
	{

	}

	Triton::Core::Shader* CreateShader() override
	{
		return new Triton::Core::Shader("D:/Programming/Test files/nfw/shaders/v0.shader", "D:/Programming/Test files/nfw/shaders/f0.shader");
	}
};

Triton::Application* Triton::CreateApplication()
{
	return new Sandbox();
}