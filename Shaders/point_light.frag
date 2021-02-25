#version 330 core
in vec3 f_Normal;
in vec2 f_TexCoord;
in vec3 f_Position;

uniform float u_Time;
uniform sampler2D u_Sampler0;
uniform sampler2D u_Sampler1;

out vec4 o_Color;

void main()
{
	o_Color = vec4(1.0f);
}