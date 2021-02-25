#include "Camera.h"
#include "Globals.h"
#include <glm/gtc/quaternion.hpp>

void CamUpdate()
{
	if (!cam_control)
	{
		return;
	}

	// Rotation
	cam.yaw -= mouse_delta.x * CAM_SENSITIVITY;
	cam.pitch -= mouse_delta.y * CAM_SENSITIVITY;

	// Movement
	glm::vec3 move = glm::vec3(0.0f);
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		move += CamRight();
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		move -= CamRight();
	}

	if (glfwGetKey(window, GLFW_KEY_W))
	{
		move += CamForward();
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		move -= CamForward();
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		move += glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
	{
		move -= glm::vec3(0.0f, 1.0f, 0.0f);
	}

	cam.position += move * delta_time * CAM_SPEED;
}

glm::mat4 CamMatrix()
{
	glm::mat4 view_matrix = lookAt(cam.position, cam.position + CamForward(), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection_matrix;

	if (cam.perspective)
	{
		projection_matrix = glm::perspective(glm::radians(cam.fov), cam.aspect, cam.near, cam.far);
	}
	else
	{
		float halfHeight = cam.ortho_height * 0.5f;
		projection_matrix = glm::ortho(-halfHeight * cam.aspect, halfHeight * cam.aspect, -halfHeight, halfHeight, cam.near, cam.far);
	}

	return projection_matrix * view_matrix;
}

glm::quat CamQuat()
{
	return angleAxis(cam.yaw, glm::vec3(0.0f, 1.0f, 0.0f)) * angleAxis(cam.pitch, glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 CamForward()
{
	return CamQuat() * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec3 CamRight()
{
	return CamQuat() * glm::vec3(1.0f, 0.0f, 0.0f);
}
