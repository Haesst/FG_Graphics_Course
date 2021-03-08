#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Mesh.h"
#include "Globals.h"

using namespace glm;

void HandleKeyEvent(GLFWwindow* window, int key, int scancode, int action, int modifiers)
{
	if (action != GLFW_PRESS)
	{
		return;
	}

	if (key == GLFW_KEY_SPACE)
	{
		printf("Wahoo!\n");
	}

	if (key == GLFW_KEY_W)
	{
		printf("Haiyaa!\n");
	}

	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_ENTER && modifiers == GLFW_MOD_ALT)
	{
		int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);

		if (maximized)
		{
			glfwRestoreWindow(window);
		}
		else
		{
			glfwMaximizeWindow(window);
		}
	}
}

void HandleMouseEvent(GLFWwindow* window, int button, int action, int modifier)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			cam_control = true;
		}

		if (action == GLFW_RELEASE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cam_control = false;
		}
	}
}

void HandleMousePos(GLFWwindow* window, double mouse_x, double mouse_y)
{
	mouse_position = vec2(mouse_x, mouse_y);
}

GLuint Create_Shader(const char* source, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// Error checking :)
	static char BUFFER[1024];
	glGetShaderInfoLog(shader, 1024, NULL, BUFFER);
	printf("-- Compiling Shader %d --\n", type);
	printf(BUFFER);

	return shader;
}

GLuint Create_Shader_Program(const char* vert_src, const char* frag_src)
{
	GLuint vertex_shader = Create_Shader(vert_src, GL_VERTEX_SHADER);

	GLuint fragment_shader = Create_Shader(frag_src, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	return program;
}

GLuint Create_VAO_For_Data(float* vertex_data, int data_size)
{
	// Vertex buffer object - the actual buffer data
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, data_size, vertex_data, GL_STATIC_DRAW);

	GLuint vao;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLsizei stride = sizeof(float) * 7;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, stride, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, stride, (void*)(5 * sizeof(float)));

	return vao;
}

GLuint Create_EBO_For_Data(GLuint* indicies_data, int data_size)
{
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, indicies_data, GL_STATIC_DRAW);

	return ebo;
}

void Draw(GLuint program, GLuint vertices, GLuint indicies, GLuint indexCount, GLenum drawMode, const void* offset)
{
	glUseProgram(program);
	glBindVertexArray(vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
	glDrawElements(drawMode, indexCount, GL_UNSIGNED_INT, offset);
}

GLuint Load_Texture(const char* path)
{
	// Textures! Very cool
	int width, height;
	int channels;
	stbi_uc* tex_data = stbi_load(path, &width, &height, &channels, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return texture;
}

void Handle_Framebuffer_Resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	cam.aspect = width / (float)height;
}

int main()
{
	glfwInit();

	window = glfwCreateWindow(1024, 768, "Graphics are kewl", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Handle_Framebuffer_Resize);

	glewInit();

	glfwSetKeyCallback(window, HandleKeyEvent);
	glfwSetMouseButtonCallback(window, HandleMouseEvent);
	glfwSetCursorPosCallback(window, HandleMousePos);

	glEnable(GL_DEPTH_TEST);

	// GLuint program = Create_Shader_Program(VERT_SRC, PurpleTetraminoShader);
	Material mat = LoadMaterial("Shaders/square.vert", "Shaders/square.frag");
	MaterialAddTexture(&mat, Load_Texture("Assets/texture.jpg"));
	MaterialUse(mat);

	Material mat2 = LoadMaterial("Shaders/light.vert", "Shaders/light.frag");
	MaterialAddTexture(&mat2, Load_Texture("Assets/texture2.jpg"));

	Material mat_pointLight = LoadMaterial("Shaders/light.vert", "Shaders/point_light.frag");

	// Mesh loading
	Mesh alien = Mesh_Load("Assets/Alien Animal.obj");
	Mesh spaceship = Mesh_Load("Assets/Spaceship.obj");
	Mesh sphere = Mesh_Load("Assets/SmoothSphere.obj");
	Mesh monkey = Mesh_Load("Assets/Monkey.obj");
	Mesh cube = Mesh_Load("Assets/Cube.obj");

	// Set initial aspect ratio of camera
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	cam.aspect = width / (float)height;

	// For calculating delta time and mouse delta
	float last_time = glfwGetTime();
	vec2 last_mouse_position = mouse_position;

	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		float time = glfwGetTime();
		delta_time = time - last_time;
		last_time = time;
		MaterialSet("u_Time", time);

		// Calculate delta mouse movement
		mouse_delta = mouse_position - last_mouse_position;
		last_mouse_position = mouse_position;

		// Update camera
		CamUpdate();

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw nice meshes!
		Render_Data render_data;
		render_data.eye_position = cam.position;
		render_data.view_projection = CamMatrix();
		render_data.material = &mat2;

		render_data.directional_light = normalize(vec3(-1.0, -1.0f, -1.0f));
		render_data.model = translate(identity<mat4>(), vec3(0.0f, -1.0f, 0.0f)) * scale(identity<mat4>(), vec3(0.1f));

		//render_data.point_light.position = vec3(0.0f, 0.0f, 3.0f);
		render_data.point_light[0].position = vec3(cos(time / 3.0f), 0.0f, sin(time)) * 5.0f;
		render_data.point_light[0].color = vec3(1.0f, 0.0f, 0.0f);
		render_data.point_light[0].intensity = cos(time * 2.0f) * 0.5f + 1.5f;

		render_data.point_light[1].position = vec3(cos(time * 1.6f), cos(time * 0.7f) * 0.4f, sin(time * 1.6)) * 4.0f;
		render_data.point_light[1].color = vec3(0.0f, 0.6f, 0.6f);
		render_data.point_light[1].intensity = 0.8f;


		Mesh_Draw(alien, render_data);

		render_data.model = translate(identity<mat4>(), vec3(0.0f, -2.5f, 0.0f)) * scale(mat4(1.0f), vec3(25.0f, 0.5f, 25.0f));
		Mesh_Draw(cube, render_data);

		render_data.model = translate(identity<mat4>(), vec3(3.5f, 0.0f, 0.0f)) * scale(identity<mat4>(), vec3(0.4f));
		Mesh_Draw(spaceship, render_data);

		render_data.material = &mat2;
		//render_data.model = translate(identity<mat4>(), vec3(-2.5f, 0.0f, 0.0f));
		render_data.model = translate(identity<mat4>(), vec3(-3.5f, 0.0f, 0.0f)) * rotate(identity<mat4>(), time * 8.0f, vec3(0.0f, 1.0f, 0.0f));
		Mesh_Draw(monkey, render_data);

		for (int i = 0; i < 2; ++i)
		{
			render_data.model = translate(identity<mat4>(), render_data.point_light[i].position) * scale(mat4(1.0f), vec3(0.1f));
			render_data.material = &mat_pointLight;
			Mesh_Draw(sphere, render_data);
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}