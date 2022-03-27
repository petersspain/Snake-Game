#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture {
public:
	Texture();
	~Texture();

	void Generate(size_t width, size_t height, const unsigned char* data);

	// may be used before Generate()
	void SetWrapS(size_t param) { wrap_s_ = param; }
	void SetWrapT(size_t param) { wrap_t_ = param; }
	void SetInternalFormat(size_t param) { internal_format_ = param; }
	void SetImageFormat(size_t param) { image_format_ = param; }
	void SetFilterMin(size_t param) { filter_min_ = param; }
	void SetFilterMax(size_t param) { filter_max_ = param; }

	size_t GetTextureID() const { return texture_id_; }
	size_t GetWidth() const { return width_; }
	size_t GetHeight() const { return height_; }

	void Bind() const;
private:
	size_t texture_id_ = 0;
	// texture image dimensions
	size_t width_ = 0, height_ = 0; // width and height of loaded image in pixels
	// texture Format
	size_t internal_format_ = GL_RGB; // format of texture object
	size_t image_format_ = GL_RGB; // format of loaded image
	// texture configuration
	size_t wrap_s_ = GL_REPEAT; // wrapping mode on S axis
	size_t wrap_t_ = GL_REPEAT; // wrapping mode on T axis
	size_t filter_min_ = GL_LINEAR; // filtering mode if texture pixels < screen pixels
	size_t filter_max_ = GL_LINEAR; // filtering mode if texture pixels > screen pixels
};