#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
	Window();
	Window(int width, int height, const char* title);

	bool GetShouldClose() const { //inline
		return glfwWindowShouldClose(window_);
	}
	void ProcessInput();

	void SwapBuffers() { // inline
		glfwSwapBuffers(window_);
	}

	~Window();
private:
	void Initialize();
	void Clear();
private:
	GLFWwindow* window_ = nullptr; // should be created in default constructor via initialization
	int width_ = 800, height_ = 600;
	const char* title_ = "Window";
};