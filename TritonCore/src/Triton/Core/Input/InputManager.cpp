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
		this->Post(new KeyPressedEvent(key, m_keyRepeat));
	}
	else
	{
		m_keyboardState->Keys[key] = false;
		this->Post(new KeyReleasedEvent(key));
	}

	m_lastLey = key;
}

void Triton::Core::InputManager::charInput(unsigned int chr)
{
	this->Post(new KeyInputEvent(chr));
}

void Triton::Core::InputManager::mouseKeyInput(InputType type, int key)
{
	if (type == InputType::PRESSED)
	{
		m_mouseState->Keys[key] = true;
		this->Post(new MouseButtonPressedEvent(key));
	}
	else
	{
		m_mouseState->Keys[key] = false;
		this->Post(new MouseButtonReleasedEvent(key));
	}
}

void Triton::Core::InputManager::mouseMoveInput(double xdelta, double ydelta)
{
	m_mouseState->XPosDelta = xdelta;
	m_mouseState->YPosDelta = ydelta;

	this->Post(new MouseMovedEvent(xdelta, ydelta));
}

void Triton::Core::InputManager::mouseScrollInput(double xdelta, double ydelta)
{
	m_mouseState->XScrollDelta = xdelta;
	m_mouseState->YScrollDelta = ydelta;

	m_mouseState->ScrollX += xdelta;
	m_mouseState->ScrollY += ydelta;

	this->Post(new MouseScrolledEvent(xdelta, ydelta));
}

Triton::relay_ptr<Triton::Keyboard> Triton::Core::InputManager::getKeyboard()
{
	return m_keyboardState;
}

Triton::relay_ptr<Triton::Mouse> Triton::Core::InputManager::getMouse()
{
	return m_mouseState;
}

void Triton::Core::InputManager::gotFocus()
{
	this->Post(new WindowFocusedEvent());
	m_focus = true;
}

void Triton::Core::InputManager::lostFocus()
{
	this->Post(new WindowLostFocusEvent());
	m_focus = false;

	// Disable all mouse keys
	// Only affects polling
	for (int i = 0; i < TR_MOUSE_KEY_COUNT; i++)
	{
		m_mouseState->Keys[i] = false;
	}

	// Disable all keyboard keys
	// Only affects polling
	for (int i = 0; i < TR_KEYBOARD_KEY_COUNT; i++)
	{
		m_keyboardState->Keys[i] = false;
	}
}

void Triton::Core::InputManager::onRegistered()
{
}

void Triton::Core::InputManager::onUnRegistered()
{
}

