#include "Shader.h"
#include <stdio.h>

GLuint load_shader(const char* path, GLenum type)
{
	printf("Loading shader '%s'\n", path);

	// Load the file
	FILE* file;

	fopen_s(&file, path, "r");

	static char BUFFER[1024];
	int file_size = fread(BUFFER, 1, 1024, file);
	fclose(file);

	BUFFER[file_size] = 0;

	// Create the shader opengl object
	char* buffer_ptr = BUFFER;

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &buffer_ptr, NULL);
	glCompileShader(shader);

	// Print result of compilation
	glGetShaderInfoLog(shader, 1024, NULL, BUFFER);
	printf(BUFFER);

	return shader;
}

GLuint load_shader_program(const char* vert_path, const char* frag_path)
{
	GLuint vert_shader = load_shader(vert_path, GL_VERTEX_SHADER);
	GLuint frag_shader = load_shader(frag_path, GL_FRAGMENT_SHADER);

	// Create the program
	GLuint program = glCreateProgram();

	// Attach and link shaders
	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);
	
	return program;
}