#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.h"

extern GLFWwindow* window;

// Time stuff
extern float delta_time;

// Mouse stuff
extern glm::vec2 mouse_position;
extern glm::vec2 mouse_delta;

// Camera stuff
extern Camera cam;
extern bool cam_control;