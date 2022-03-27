#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include "Window.h"
#include "Game.h"

// GL MATH LIBRARY
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const int WIDTH = 800; // 1366
const int HEIGHT = 600; // 768
const char* TITLE = "Snake Game";

Game Snake(WIDTH, HEIGHT);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Snake.SetKey(key, true);
		else if (action == GLFW_RELEASE)
			Snake.SetKey(key, false);
	}
}

//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	std::cout << "Window size was changed!" << std::endl;
//	glViewport(0, 0, width, height);
//}

int main() {
	// Create Window
	Window window(WIDTH, HEIGHT, TITLE);
	window.SetKeyCallback(key_callback);

	// Callback on changing window size
	// glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Compile shaders
	//Shader shader_program("Shaders/shader.vertex", "Shaders/shader.fragment");

	//// Create triangle MESH
	//float vertices[] = {
	//	// positions          // colors           // texture coords
	//	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	//	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	//   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	//   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	//};
	//size_t indices[] = {
	//	0, 1, 3,
	//	1, 2, 3
	//};
	//Mesh square(vertices, sizeof(vertices), indices, sizeof(indices));
	//Texture snake_block("Textures/snake_block.png", GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE0);

	//float scale_factor = 100.f;
	//float x_trans = 0.f;
	//float y_trans = HEIGHT / 2;

	Snake.Init();

	float delta_time = 0.0f;
	float last_frame = 0.0f;
	float current_frame = 0.0f;

	while (!window.GetShouldClose()) {
		// calculate delta time
		current_frame = static_cast<float>(glfwGetTime());
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		// input
		glfwPollEvents();

		// manage user input
		Snake.ProcessInput(delta_time);

		// update game state
		Snake.Update(delta_time);

		// rendering commands
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Snake.Render();

		window.SwapBuffers(); // swap back and front buffers
	}
	ResourceManager::Clear();
	return 0;
}
