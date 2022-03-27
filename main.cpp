#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Window.h"
#include "Shader.h"
#include "Texture.h"

// GL MATH LIBRARY
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	size_t indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	Mesh square(vertices, sizeof(vertices), indices, sizeof(indices));
	Texture snake_block("Textures/snake_block.png", GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE0);

	float scale_factor = 100.f;
	float x_trans = 0.f;
	float y_trans = HEIGHT / 2;

	while (!window.GetShouldClose()) {
		// input
		window.ProcessInput();
		//if (window.GetKeyStatus(GLFW_KEY_W) == GLFW_PRESS) {
		//	scale_factor += 1.0f;
		//}
		//else if (window.GetKeyStatus(GLFW_KEY_S) == GLFW_PRESS) {
		//	scale_factor -= 1.0f;
		//}

		//if (window.GetKeyStatus(GLFW_KEY_UP) == GLFW_PRESS) {
		//	y_trans += 1.f;
		//}
		//else if (window.GetKeyStatus(GLFW_KEY_DOWN) == GLFW_PRESS) {
		//	y_trans -= 1.f;
		//}

		if (window.GetKeyStatus(GLFW_KEY_RIGHT) == GLFW_PRESS) {
			x_trans += 1.f;
		}
		else if (window.GetKeyStatus(GLFW_KEY_LEFT) == GLFW_PRESS) {
			x_trans -= 1.0f;
		}

		if (x_trans > WIDTH) {
			x_trans = 0.f;
		}

		//if (scale_factor > 1000.f) {
		//	scale_factor = 1000.f;
		//}
		//if (scale_factor < 0.f) {
		//	scale_factor = 0.f;
		//}

		// rendering commands
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw
		shader_program.Use(); // what shaders should we use
		shader_program.SetInt("texture0", 0);
		snake_block.Use();

		// first block
		// model
		for (size_t i = 0; i < 2; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(scale_factor, scale_factor, 1.f));
			shader_program.SetMatrix4f("model", model);

			// view
			glm::mat4 view = glm::mat4(1.f);
			view = glm::translate(view, glm::vec3(x_trans + i * scale_factor, y_trans, 0.f));
			shader_program.SetMatrix4f("view", view);

			// projection
			glm::mat4 projection = glm::ortho(0.f, static_cast<float>(WIDTH), static_cast<float>(HEIGHT), 0.f, -1.f, 1.f);
			shader_program.SetMatrix4f("projection", projection);

			//render
			square.Render(); // what object we render - which vertices we sending as input in the VERTEX SHADER
		}

		// check and call events and swap buffers
		glfwPollEvents();
		window.SwapBuffers(); // swap back and front buffers
	}
	// glDeleteProgram(shader_program); called in ShaderProgram-class destructor

	// glfwTerminate(); called in Window-class destructor 
	return 0;
}