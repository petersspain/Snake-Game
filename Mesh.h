#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Mesh {
public:
	explicit Mesh(GLfloat* vertices, size_t num_of_vertices, size_t* indices, size_t num_of_indices);

	void Render() const;

	~Mesh();
private:
	GLuint VAO_ = 0, VBO_ = 0, EBO_ = 0;
	GLsizei index_count_ = 0;
};