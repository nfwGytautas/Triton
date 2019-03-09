#include "TRpch.h"
#include "EventReceiver.h"

#include "Triton\Events\Events.h"
#include "Triton\Events\KeyEvent.h"
#include "Triton\Events\MouseEvent.h"
#include "Triton\Events\ApplicationEvent.h"

void Triton::Core::EventReceiver::OnKeyPressed(int aKeycode, int aRepeatCount, int aScancode, int aMods)
{
	OnEvent(new KeyPressedEvent(aKeycode, aRepeatCount, aScancode, aMods));
}

void Triton::Core::EventReceiver::OnKeyReleased(int aKeycode, int aScancode, int aMods)
{
	OnEvent(new KeyReleasedEvent(aKeycode, aScancode, aMods));
}

void Triton::Core::EventReceiver::OnMouseMoved(double aX, double aY)
{
	OnEvent(new MouseMovedEvent(aX, aY));
}

void Triton::Core::EventReceiver::OnMouseButtonPressed(int aKeycode)
{
	OnEvent(new MouseButtonPressedEvent(aKeycode));
}

void Triton::Core::EventReceiver::OnMouseButtonReleased(int aKeycode)
{
	OnEvent(new MouseButtonReleasedEvent(aKeycode));
}

void Triton::Core::EventReceiver::OnMouseScrolled(double aXOffset, double aYOffset)
{
	OnEvent(new MouseScrolledEvent(aXOffset, aYOffset));
}

void Triton::Core::EventReceiver::OnKeyInput(unsigned int aCharInput)
{
	OnEvent(new KeyInputEvent(aCharInput));
}

void Triton::Core::EventReceiver::OnWindowResized(int aWidth, int aHeight)
{
	OnEvent(new WindowResizeEvent(aWidth, aHeight));
}
