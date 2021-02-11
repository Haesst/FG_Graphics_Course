#version 330 core
uniform vec4 u_Color_a;
uniform vec4 u_Color_b;
uniform float u_Time;

out vec4 o_Color;

void main()
{
	o_Color = abs(mix(u_Color_a, u_Color_b, sin(u_Time)));
}