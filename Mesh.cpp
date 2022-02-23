#include "Mesh.h"

Mesh::Mesh(GLfloat* vertices, size_t num_of_vertices, size_t* indices, size_t num_of_indices) 
	: index_count_(num_of_indices) {
	glGenVertexArrays(1, &VAO_); // vertex array object
	glBindVertexArray(VAO_);

	glGenBuffers(1, &VBO_); // vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, num_of_vertices, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO_); // element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_of_indices, indices, GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unbind all objects
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// UNBIND EBO AFTER UNBINDING VAO !!!!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Render() const {
	glBindVertexArray(VAO_);
	glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh() {
	if (EBO_ != 0) {
		glDeleteBuffers(1, &EBO_);
	}
	if (VBO_ != 0) {
		glDeleteBuffers(1, &VBO_);
	}
	if (VAO_ != 0) {
		glDeleteVertexArrays(1, &VAO_);
	}
}