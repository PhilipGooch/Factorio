// Procedural
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_GridTexture;

void main()
{
	color = vec4(0, 0, 0, 1);


	if (int(texture(u_GridTexture, v_TexCoord).r * 255) == 0)
	{
		color = vec4(1, 1, 1, 1);
	}
	else if (int(texture(u_GridTexture, v_TexCoord).r * 255) == 1)
	{
		color = vec4(0, 0, 1, 1);
	}
	else if (int(texture(u_GridTexture, v_TexCoord).r * 255) == 2)
	{
		color = vec4(0, 1, 0, 1);
	}
	else if (int(texture(u_GridTexture, v_TexCoord).r * 255) == 3)
	{
		color = vec4(1, 0, 0, 1);
	}
};