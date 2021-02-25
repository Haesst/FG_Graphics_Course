#pragma once
#include <glm/glm.hpp>

const float CAM_SENSITIVITY = 0.003f;
const float CAM_SPEED = 4.0f;

struct Camera
{
	float yaw;
	float pitch;

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);

	bool perspective = true;
	float aspect = 1.0f;
	float fov = 60.0f;

	float near = 0.1f;
	float far = 100.0f;

	float ortho_height = 5.0f;
};

void CamUpdate();
glm::mat4 CamMatrix();
glm::quat CamQuat();
glm::vec3 CamForward();
glm::vec3 CamRight();