// Procedural
#version 330 core

layout(location = 0) in vec2 gridPos;
layout(location = 1) in float type;

out vec2 v_TexCoord;
flat out int v_Type;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

uniform float u_TileExtent;

void main()
{
	mat4 MVP = u_Projection * u_View * u_Model;
	gl_Position = MVP * vec4(gridPos.x * u_TileExtent, gridPos.y * u_TileExtent, 0, 1);
	v_Type = int(type);
};