#include "TRpch.h"
#include "InputManager.h"

Triton::Core::InputManager::InputManager()
{
	m_keyboardState = new Keyboard();
	m_mouseState = new Mouse();
}

Triton::Core::InputManager::~InputManager()
{
	delete m_keyboardState;
	delete m_mouseState;
}

void Triton::Core::InputManager::keyInput(InputType type, int key)
{
	if (m_lastLey == key)
	{
		m_keyRepeat++;
	}
	else
	{
		m_keyRepeat = 0;
	}

	if (type == InputType::PRESSED)
	{
		m_keyboardState->Keys[key] = true;
		m_eManager->Post(new KeyPressedEvent(key, m_keyRepeat));
	}
	else
	{
		m_keyboardState->Keys[key] = false;
		m_eManager->Post(new KeyReleasedEvent(key));
	}

	m_lastLey = key;
}

void Triton::Core::InputManager::charInput(unsigned int chr)
{
	m_eManager->Post(new KeyInputEvent(chr));
}

void Triton::Core::InputManager::mouseKeyInput(InputType type, int key)
{
	if (type == InputType::PRESSED)
	{
		m_mouseState->Keys[key] = true;
		m_eManager->Post(new MouseButtonPressedEvent(key));
	}
	else
	{
		m_mouseState->Keys[key] = false;
		m_eManager->Post(new MouseButtonReleasedEvent(key));
	}
}

void Triton::Core::InputManager::mouseMoveInput(double xdelta, double ydelta)
{
	m_mouseState->XPosDelta = xdelta;
	m_mouseState->YPosDelta = ydelta;

	m_eManager->Post(new MouseMovedEvent(xdelta, ydelta));
}

void Triton::Core::InputManager::mouseScrollInput(double xdelta, double ydelta)
{
	m_mouseState->XScrollDelta = xdelta;
	m_mouseState->YScrollDelta = ydelta;

	m_mouseState->ScrollX += xdelta;
	m_mouseState->ScrollY += ydelta;

	m_eManager->Post(new MouseScrolledEvent(xdelta, ydelta));
}

void Triton::Core::InputManager::setEventManager(reference<Core::EventManager> manager)
{
	m_eManager = manager;
}

Triton::relay_ptr<Triton::Keyboard> Triton::Core::InputManager::getKeyboard()
{
	return m_keyboardState;
}

Triton::relay_ptr<Triton::Mouse> Triton::Core::InputManager::getMouse()
{
	return m_mouseState;
}
