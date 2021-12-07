// Procedural
#version 330 core

layout(location = 0) in vec2 gridPosition;
layout(location = 1) in float type;
layout(location = 2) in float targeted;

flat out int v_Type;
flat out int v_Targeted;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

uniform float u_TileExtent;

void main()
{
	mat4 MVP = u_Projection * u_View * u_Model;
	gl_Position = MVP * vec4(gridPosition.x * u_TileExtent, gridPosition.y * u_TileExtent, 0, 1);
	v_Type = int(type);
	v_Targeted = int(targeted);
};