#pragma once

class Input
{
public:

	bool getKeyDown(int key)
	{
		if (key >= 65 && key <= 90)
			return keys[key - 65];
		else
			return false;
	}

	void setKeyDown(int key)
	{
		if (key >= 65 && key <= 90)
			keys[key - 65] = true;
	}

	void setKeyUp(int key)
	{
		if (key >= 0)
			keys[key - 65] = false;
	}

private:

	bool keys[256]{ false };

};