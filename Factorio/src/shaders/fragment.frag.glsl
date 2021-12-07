// Procedural
#version 330 core

layout(location = 0) out vec4 color;

flat in int v_Type;
flat in int v_Targeted;

void main()
{
	color = vec4(0, 0, 0, 1);

	if (v_Targeted > 0)
	{
		color = vec4(1, 0, 0, 1);
	}
	else
	{
		if (v_Type == 0)
		{
			color = vec4(48 / 225.0f, 46 / 225.0f, 145 / 225.0f, 1);
		}
		else if (v_Type == 1)
		{
			color = vec4(202 / 225.0f, 180 / 225.0f, 112 / 225.0f, 1);
		}
		else if (v_Type == 2)
		{
			color = vec4(92 / 225.0f, 120 / 225.0f, 22 / 225.0f, 1);
		}
		else if (v_Type == 3)
		{
			color = vec4(133 / 225.0f, 133 / 225.0f, 124 / 225.0f, 1);
		}
	}

};