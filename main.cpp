#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int WIDTH = 800;
const int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::cout << "Window size was changed!" << std::endl;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

std::string ReadFile(const char* flocation) {
	std::string content;
	std::ifstream fstream(flocation, std::ios::in);
	if (!fstream.is_open()) {
		std::cerr << "Failed to read " << flocation << "! File doesnt exist.";
	}
	std::string line = "";
	while (!fstream.eof()) {
		std::getline(fstream, line);
		content.append(line + "\n");
	}
	fstream.close();
	return content;
}

void CompileShader(const char* filename, size_t shader_id, GLenum shader_type) {
	std::string shader_code = ReadFile(filename);

	const GLchar* the_code[1];
	the_code[0] = shader_code.c_str();

	GLint code_length[1];
	code_length[0] = shader_code.size();

	glShaderSource(shader_id, 1, the_code, code_length);
	glCompileShader(shader_id);

	GLint result = 0;
	GLchar info_log[1024] = { 0 };

	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader_id, sizeof(info_log), NULL, info_log);
		std::cerr << "Error compiling the " << shader_type << " shader: " << info_log << std::endl;
		return;
	}
}

int main() {
	// GLFW initialisation
	if (!glfwInit()) {
		std::cerr << "GLFW initialisation failed" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SnakeGame", 0, 0);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 2;
	}
	glfwMakeContextCurrent(window);

	// GLEW initialisation
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW initialisation failed!" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return 3;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	// Callback on changing window size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Compile shaders
	size_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	CompileShader("Shaders/shader.vertex", vertex_shader, GL_VERTEX_SHADER);

	size_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	CompileShader("Shaders/shader.fragment", fragment_shader, GL_FRAGMENT_SHADER);

	// Create shader program
	size_t shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	GLint result = 0;
	GLchar info_log[1024] = { 0 };
	glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
		std::cerr << "Error linking program: " << info_log << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return 4;
	}	
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	float vertices[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	size_t indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	size_t VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// UNBIND EBO AFTER UNBINDING VAO !!!!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// check and call events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader_program);

	glfwTerminate();
	return 0;
}