#pragma once

#include <glm/glm.hpp>

class Input
{
public:

	bool GetKeyDown(int key)
	{
		if (key >= 65 && key <= 90)
			return m_Keys[key - 65];
		else
			return false;
	}

	void SetKeyDown(int key)
	{
		if (key >= 65 && key <= 90)
			m_Keys[key - 65] = true;
	}

	void SetKeyUp(int key)
	{
		if (key >= 0)
			m_Keys[key - 65] = false;
	}

	bool GetMouseDown() { return m_MouseDown; }
	void SetMouseDown(bool mouseDown) { m_MouseDown = mouseDown; }

	glm::vec2 GetMousePosition() { return m_MousePosition; }
	void SetMousePosition(int x, int y) { m_MousePosition.x = x; m_MousePosition.y = y; }

private:

	glm::vec2 m_MousePosition;
	bool m_MouseDown;

	bool m_Keys[256]{ false };

};