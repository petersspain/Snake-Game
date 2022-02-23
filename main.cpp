#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Window.h"
#include "Shader.h"

const int WIDTH = 800; // 1366
const int HEIGHT = 600; // 768
const char* TITLE = "Snake Game";

//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	std::cout << "Window size was changed!" << std::endl;
//	glViewport(0, 0, width, height);
//}

int main() {
	// Create Window
	Window window(WIDTH, HEIGHT, TITLE);

	// Callback on changing window size
	// glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Compile shaders
	Shader shader_program("Shaders/shader.vertex", "Shaders/shader.fragment");

	// Create triangle MESH
	float vertices[] = {
		 // positions        // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};
	size_t indices[] = {
		1, 2, 3,
	};
	Mesh triangle(vertices, sizeof(vertices), indices, sizeof(indices));

	while (!window.GetShouldClose()) {
		// input
		window.ProcessInput();

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		shader_program.Use(); // what shaders should we use
		triangle.Render(); // what object we render - which vertices we sending as input in the VERTEX SHADER

		// check and call events and swap buffers
		glfwPollEvents();
		window.SwapBuffers(); // swap back and front buffers
	}
	// glDeleteProgram(shader_program); called in ShaderProgram-class destructor

	// glfwTerminate(); called in Window-class destructor 
	return 0;
}