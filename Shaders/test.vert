#version 330 core
uniform float u_Scale;
uniform float u_XpositionMod;

in vec2 a_Position;

void main()
{
    vec2 position = vec2(a_Position.x * u_Scale, a_Position.y * u_Scale);
    position.x += u_XpositionMod;

	gl_Position = vec4(position, 0.0, 1.0);
}