#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

const int MAX_TEXTURES = 4;

struct Material
{
	GLuint program;
	GLuint textures[MAX_TEXTURES];
	int texture_number = 0;
};

Material LoadMaterial(const char* vert_path, const char* frag_path);
void MaterialAddTexture(Material* mat, GLuint texture);
// Helper functions
void MaterialUse(Material mat);
void MaterialSet(const char* name, float value);
void MaterialSet(const char* name, const glm::mat4& value);
void MaterialSet(const char* name, const glm::vec3& value);