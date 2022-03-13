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
	Mesh triangle(vertices, sizeof(vertices), indices, sizeof(indices));
	Texture wooden_container("Textures/container.jpg");
	wooden_container.ChangeTextureWrapTo(GL_CLAMP_TO_EDGE);
	Texture awesome_face("Textures/awesomeface.png", GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE1);

	wooden_container.ChangeTextureMinFilterTo(GL_NEAREST);
	wooden_container.ChangeTextureMagFilterTo(GL_NEAREST);

	awesome_face.ChangeTextureMinFilterTo(GL_NEAREST);
	awesome_face.ChangeTextureMagFilterTo(GL_NEAREST);

	float mix_factor = 0.5f;

	while (!window.GetShouldClose()) {
		// input
		window.ProcessInput();
		if (window.GetKeyStatus(GLFW_KEY_UP) == GLFW_PRESS) {
			mix_factor += 0.01f;
		}
		else if (window.GetKeyStatus(GLFW_KEY_DOWN) == GLFW_PRESS) {
			mix_factor -= 0.01f;
		}

		if (mix_factor > 1.0) {
			mix_factor = 1.0f;
		}
		if (mix_factor < 0.0) {
			mix_factor = 0.0f;
		}

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// create transform
		//transform = glm::rotate(transform, static_cast<float>(glfwGetTime()), glm::vec3(0.f, 0.f, 1.f));
		//transform = glm::scale(transform, glm::vec3(1.5f, 1.5f, 1.0f));

		// draw our first triangle
		shader_program.Use(); // what shaders should we use
		shader_program.SetInt("texture0", 0);
		shader_program.SetInt("texture1", 1);
		shader_program.SetFloat("mix_factor", mix_factor);
		wooden_container.Use();
		awesome_face.Use();

		glm::mat4 transform = glm::mat4(1.0f);
		shader_program.SetMatrix4f("transform", transform);
		triangle.Render(); // what object we render - which vertices we sending as input in the VERTEX SHADER

		float scl_factor = glm::sin(static_cast<float>(glfwGetTime()));
		transform = glm::translate(transform, glm::vec3(scl_factor, scl_factor, 0.0f));
		transform = glm::scale(transform, glm::vec3(scl_factor, scl_factor, 1.0f));
		shader_program.SetMatrix4f("transform", transform);
		triangle.Render();

		// check and call events and swap buffers
		glfwPollEvents();
		window.SwapBuffers(); // swap back and front buffers
	}
	// glDeleteProgram(shader_program); called in ShaderProgram-class destructor

	// glfwTerminate(); called in Window-class destructor 
	return 0;
}