#pragma once
#include "wavefront.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Material;

struct Point_Light
{
	glm::vec3 position = glm::vec3(0.0f);
	float intensity = 1.0f;
};

struct Render_Data
{
	glm::mat4 view_projection;
	glm::mat4 model = glm::identity<glm::mat4>();

	Material* material = nullptr;

	// Lights
	glm::vec3 directional_light;
	glm::vec3 eye_position;

	Point_Light point_light;
};

struct Mesh
{
	GLuint vao;
	int draw_count;
};

Mesh Mesh_Load(const char* path);
void Mesh_Draw(const Mesh& mesh, Render_Data& data);