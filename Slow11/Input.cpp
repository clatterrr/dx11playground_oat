#include "Input.h"
#include "SysDef.h"

Input::Input()
{
	memset(m_mouseDelta, 0, 2 * sizeof(int));
	m_mousePos[0] = SCREEN_WIDTH / 2;
	m_mousePos[1] = SCREEN_HEIGHT / 2;
}

Input::Input(const Input&)
{
}


Input::~Input()
{
}

void Input::Init()
{
	for (size_t i = 0; i < KEY_COUNT; ++i)
		m_keys[i] = false;
}

void Input::KeyDown(KeyCode key)
{
	m_keys[key] = true;
}
void Input::KeyUp(KeyCode key)
{
	m_keys[key] = false;
}
void Input::ButtonDown(KeyCode btn)
{
	m_buttons[btn] = true;
}
void Input::ButtonUp(KeyCode btn)
{
	m_buttons[btn] = false;
}

void Input::UpdateMousePos(int x, int y)
{
	m_mouseDelta[0] = x - m_mousePos[0];
	m_mouseDelta[1] = y - m_mousePos[1];
	m_mousePos[0] = x;
	m_mousePos[1] = y;
}

bool Input::IsKeyDown(KeyCode key) const
{
	return m_keys[key];
}

bool Input::IsMouseDown(KeyCode btn) const
{
	return m_buttons[btn];
}

int Input::MouseDeltaX() const
{
	return m_mouseDelta[0];
}

int Input::MouseDeltaY() const
{
	return m_mouseDelta[1];
}
