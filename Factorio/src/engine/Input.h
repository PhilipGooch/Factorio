#pragma once

#include <glm/glm.hpp>

class Input
{
public:

	bool GetKeyDown(int key)
	{
		if (key == 256 /*Escape*/)				
			return m_Keys[key];
		if (key >= 65 && key <= 90)
			return m_Keys[key - 65];
		else
			return false;
	}

	void SetKeyDown(int key)
	{
		if (key == 256 /*Escape*/)
			m_Keys[key] = true;
		if (key >= 65 && key <= 90)
		{
			m_Keys[key - 65] = true;
		}
	}

	void SetKeyUp(int key)
	{
		if (key >= 0)
		{
			m_Keys[key - 65] = false;
		}
	}

	bool GetKeyPressed(int key)
	{
		if (key >= 0)
		{
			return !m_LastKeys[key - 65] && m_Keys[key - 65];
		}
		return false;
	}

	void Update()
	{
		memcpy(&m_LastKeys[0], &m_Keys[0], 256 * sizeof(bool));
	}

	bool GetMouseDown() { return m_MouseDown; }
	void SetMouseDown(bool mouseDown) { m_MouseDown = mouseDown; }

	glm::vec2 GetMousePosition() { return m_MousePosition; }
	void SetMousePosition(int x, int y) { m_MousePosition.x = x; m_MousePosition.y = y; }

private:

	glm::vec2 m_MousePosition;
	bool m_MouseDown;

	bool m_Keys[257]{ false };
	bool m_LastKeys[257]{ false };
};