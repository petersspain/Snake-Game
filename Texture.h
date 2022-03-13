#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// REFACTOR CLASS

class Texture {
public:
	Texture(const char* file_location, GLint format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE, GLenum texture_unit = GL_TEXTURE0);

	void Use() const;

	void ChangeTextureWrapTo(GLint param);

	void ChangeTextureMinFilterTo(GLint param);
	void ChangeTextureMagFilterTo(GLint param);
	
	~Texture();
private:
	GLenum texture_unit_;
	GLuint texture_id_ = 0;
};