#include <Triton.h>

class Sandbox : public Triton::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Triton::Application* Triton::CreateApplication()
{
	return new Sandbox();
}