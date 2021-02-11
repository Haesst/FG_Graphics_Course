#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <math.h>
#include "Shader.h"

const char* VERT_SRC = ""
"#version 330 core\n"
"in vec2 a_Position;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);\n"
"}";

const char* OrangeShader = ""
"#version 330 core\n"
"out vec4 o_Color;\n"
"void main()\n"
"{\n"
"	o_Color = vec4(1.0, 0.5, 0.2, 1.0);\n"
"}";

const char* BlueTetraminoShader = ""
"#version 330 core\n"
"out vec4 o_Color;\n"
"void main()\n"
"{\n"
"	o_Color = vec4(0.2, 1.0, 1.0, 1.0);\n"
"}";

const char* YellowTetraminoShader = ""
"#version 330 core\n"
"out vec4 o_Color;\n"
"void main()\n"
"{\n"
"	o_Color = vec4(1.0, 1.0, 0.266, 1.0);\n"
"}";

const char* OrangeTetraminoShader = ""
"#version 330 core\n"
"out vec4 o_Color;\n"
"void main()\n"
"{\n"
"	o_Color = vec4(1.0, 0.533, 0.0, 1.0);\n"
"}";

const char* GreenTetraminoShader = ""
"#version 330 core\n"
"out vec4 o_Color;\n"
"void main()\n"
"{\n"
"	o_Color = vec4(0.266, 1.0, 0.266, 1.0);\n"
"}";

const char* PurpleTetraminoShader = ""
"#version 330 core\n"
"out vec4 o_Color;\n"
"void main()\n"
"{\n"
"	o_Color = vec4(1.0, 0.266, 1.0, 1.0);\n"
"}";


const char* FRAG_SRC2 = ""
"#version 330 core\n"
"out vec4 o_Color;\n"
"void main()\n"
"{\n"
"	o_Color = vec4(0.2, 0.5, 0.8, 1.0);\n"
"}";

float SCALE = 1.0f;
float XMOD = 0.0f;

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
		glfwDestroyWindow(window);
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

	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_UP || key == GLFW_KEY_RIGHT || key == GLFW_KEY_DOWN)
	{
		int horizontal, vertical;
		glfwGetWindowPos(window, &horizontal, &vertical);

		if (key == GLFW_KEY_LEFT)
		{
			// horizontal -= 1;
			XMOD -= 0.1f;
		}

		if (key == GLFW_KEY_RIGHT)
		{
			// horizontal += 1;
			XMOD += 0.1f;
		}

		if (key == GLFW_KEY_UP)
		{
			// vertical -= 1;
			SCALE += 0.1f;
		}

		if (key == GLFW_KEY_DOWN)
		{
			// vertical += 1;
			SCALE -= 0.1f;
		}

		glfwSetWindowPos(window, horizontal, vertical);
	}
}

void HandleMouseEvent(GLFWwindow* window, int button, int action, int modifier)
{
	if (action != GLFW_PRESS)
	{
		return;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		printf("Kapow! You shot (%f, %f)\n", xPos, yPos);
	}
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

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

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

/*

Tetramino 1, blue:

	float tetraminoVerticies[] =
	{
		-0.9, 0.3,	 // 0
		 0.9, 0.3,   // 1
		-0.9, -0.3,  // 2
		 0.9, -0.3,  // 3
	};

	GLuint tetraminoIndicies[] =
	{
		0, 1, 2,
		1, 2, 3,
	};

	Yellow:
	float tetraminoVerticies[] =
	{
		-0.4, 0.4,	 // 0
		 0.4, 0.4,   // 1
		-0.4, -0.4,  // 2
		 0.4, -0.4,  // 3
	};

	GLuint tetraminoIndicies[] =
	{
		0, 1, 2,
		1, 2, 3,
	};

	Orange:

	float tetraminoVerticies[] =
	{
		-0.3, 0.5,	 // 0
		-0.1, 0.5,   // 1
		-0.3, -0.4,  // 2
		-0.1, -0.4,  // 3
		 0.05, -0.4,  // 4
		 0.05, -0.65,  // 5
		-0.3, -0.65,  // 6
	};

	GLuint tetraminoIndicies[] =
	{
		0, 1, 2,
		1, 2, 3,
		2, 4, 5,
		5, 6, 2,
	};

	Green:

	float tetraminoVerticies[] =
	{
		-0.2,  0.4, // 0
		 0.0,  0.4, // 1
		-0.2, -0.15, // 2
		 0.0, -0.15, // 3

		 0.0,  0.15, // 4
		 0.2,  0.15, // 5
		 0.0, -0.4, // 6
		 0.2, -0.4, // 7
	};

	GLuint tetraminoIndicies[] =
	{
		0, 1, 2,
		1, 2, 3,

		4, 5, 6,
		5, 6, 7,
	};
*/

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Graphics are kewl", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();

	glfwSetKeyCallback(window, HandleKeyEvent);
	glfwSetMouseButtonCallback(window, HandleMouseEvent);

	float tetraminoVerticies[] =
	{
		-0.2,  0.4, // 0
		 0.4,  0.4, // 1
		-0.2,  0.2, // 2
		 0.4,  0.2, // 3

		 0.0,  0.2, // 4
		 0.2,  0.2, // 5
		 0.0,  0.0, // 6
		 0.2,  0.0, // 7
	};

	GLuint tetraminoIndicies[] =
	{
		0, 1, 2,
		1, 2, 3,

		4, 5, 6,
		5, 6, 7,
	};


	GLuint tetraminoVAO = Create_VAO_For_Data(tetraminoVerticies, sizeof(tetraminoVerticies));
	GLuint tetraminoEBO = Create_EBO_For_Data(tetraminoIndicies, sizeof(tetraminoIndicies));

	// GLuint program = Create_Shader_Program(VERT_SRC, PurpleTetraminoShader);
	GLuint program = load_shader_program("Shaders/test.vert", "Shaders/test.frag");
	glUseProgram(program);

	GLint u_Color_a = glGetUniformLocation(program, "u_Color_a");
	GLint u_Color_b = glGetUniformLocation(program, "u_Color_b");
	GLint u_Time = glGetUniformLocation(program, "u_Time");
	GLint u_Scale = glGetUniformLocation(program, "u_Scale");
	GLint u_XpositionMod = glGetUniformLocation(program, "u_XpositionMod");
	

	float my_color_a[] = { 0.5f, 0.2f, 0.6f, 1.0f };
	glUniform4fv(u_Color_a, 1, my_color_a);

	float my_color_b[] = { 0.8f, 0.2f, 0.2f, 1.0f };
	glUniform4fv(u_Color_b, 1, my_color_b);

	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();
		glUniform1f(u_Time, time);
		glUniform1f(u_Scale, SCALE);
		glUniform1f(u_XpositionMod, XMOD);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Draw(program, tetraminoVAO, tetraminoEBO, 12, GL_TRIANGLES, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}