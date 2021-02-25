#include "Shader.h"
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

GLuint load_shader(const char* path, GLenum type)
{
	printf("Loading shader '%s'\n", path);

	// Load the file
	FILE* file;

	fopen_s(&file, path, "r");

	static char BUFFER[2048];
	int file_size = fread(BUFFER, 1, 2048, file);
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

	static char BUFFER[1024];
	glGetProgramInfoLog(program, 1024, NULL, BUFFER);
	printf(BUFFER);
	
	return program;
}

Material LoadMaterial(const char* vert_path, const char* frag_path)
{
	Material mat;
	mat.program = load_shader_program(vert_path, frag_path);

	return mat;
}

void MaterialAddTexture(Material* mat, GLuint texture)
{
	mat->textures[mat->texture_number++] = texture;
}

// Helper functions
static GLuint current_program;
void MaterialUse(Material mat)
{
	glUseProgram(mat.program);

	for (int i = 0; i < mat.texture_number; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mat.textures[i]);
	}

	current_program = mat.program;
}

void MaterialSet(const char* name, float value)
{
	GLuint location = glGetUniformLocation(current_program, name);
	glUniform1f(location, value);
}

void MaterialSet(const char* name, const glm::mat4& value)
{
	GLuint location = glGetUniformLocation(current_program, name);
	glUniformMatrix4fv(location, 1, false, value_ptr(value));
}

void MaterialSet(const char* name, const glm::vec3& value)
{
	GLuint location = glGetUniformLocation(current_program, name);
	glUniform3fv(location, 1, value_ptr(value));
}
