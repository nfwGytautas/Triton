#include "Triton2/Triton.h"

int main()
{
	using namespace Triton;

	Log::init();

	Graphics::Context* context;
	Graphics::Window* window;
	
	context = Graphics::Context::create();
	context->init();

	window = context->newWindow();
	window->initNew(1280, 720);

	window->keyboard().charInputCallback([](char c) {TR_SYSTEM_TRACE("KEY: {0}", c); });

	while (!window->isClosed())
	{
		window->update();
	}


	return 0;
}