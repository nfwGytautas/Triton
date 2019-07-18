#include "TRpch.h"
#include "EventInterface.h"

#include "Triton/Events/EventManager.h"
#include "Triton/Events/KeyEvent.h"
#include "Triton/Events/MouseEvent.h"
#include "Triton/Events/ApplicationEvent.h"

#define listen_to(eventType, function, ...) \
this->Listen<Triton::eventType>(Triton::EventBehavior(\
std::bind(\
	[&](const Triton::Event& e)\
{\
	const Triton::eventType& kpe = dynamic_cast<const Triton::eventType&>(e);\
	return this->function(__VA_ARGS__);\
},\
std::placeholders::_1)));

Triton::Utility::EventInterface::EventInterface()
	: EventListener()
{
	listen_to(KeyPressedEvent, OnKeyPressed, kpe.GetKeyCode(), kpe.GetRepeatCount(), 0, kpe.GetKeyMods());
	listen_to(KeyReleasedEvent, OnKeyReleased, kpe.GetKeyCode(), 0, kpe.GetKeyMods());
	listen_to(KeyInputEvent, OnKeyInput, kpe.GetKeyCode());


	listen_to(MouseMovedEvent, OnMouseMoved, kpe.GetX(), kpe.GetY());
	listen_to(MouseButtonPressedEvent, OnMouseButtonPressed, kpe.GetMouseButton());
	listen_to(MouseButtonReleasedEvent, OnMouseButtonReleased, kpe.GetMouseButton());
	listen_to(MouseScrolledEvent, OnMouseScrolled, kpe.GetXOffset(), kpe.GetYOffset());

	listen_to(WindowResizeEvent, OnWindowResized, kpe.GetWidth(), kpe.GetHeight());
}

Triton::Utility::EventInterface::~EventInterface()
{
}

