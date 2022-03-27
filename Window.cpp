#include "Window.h"

#include <iostream>
#include <stdexcept>

Window::Window() {
	try {
		Initialize();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		Clear();
		throw;
	}
}

Window::Window(int width, int height, const char* title) 
	: width_(width)
	, height_(height)
	, title_(title) {
	try {
		Initialize();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		Clear();
		throw;
	}
}

void Window::Initialize() {
	if (!glfwInit()) {
		throw std::runtime_error("GLFW initialisation failed");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window_ = glfwCreateWindow(width_, height_, title_, 0, 0);
	if (window_ == NULL) {
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window_);

	// GLEW initialisation
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("GLEW initialisation failed!");
	}

	glViewport(0, 0, width_, height_);
	// ?
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window() {
	Clear();
}

void Window::Clear() {
	if (window_ != nullptr) {
		glfwDestroyWindow(window_);
	}
	glfwTerminate();
}

void Window::SetKeyCallback(GLFWkeyfun function) {
	glfwSetKeyCallback(window_, function);
}
