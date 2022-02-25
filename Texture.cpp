#include "Texture.h"

#include "stb_image.h"

#include <iostream>
#include <stdexcept>

Texture::Texture(const char* file_location, GLint format, GLenum type, GLenum texture_unit)
	: texture_unit_(texture_unit) {
	int width, height, num_of_channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(file_location, &width, &height, &num_of_channels, 0);
	if (!data) {
		std::cerr << "Failed to load texture at " << file_location << std::endl;
		throw std::runtime_error("Failed to load texture");
	}

	glGenTextures(1, &texture_id_);
	glActiveTexture(texture_unit_);
	glBindTexture(GL_TEXTURE_2D, texture_id_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void Texture::Use() const {
	glActiveTexture(texture_unit_);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
}

Texture::~Texture() {
	glDeleteTextures(1, &texture_id_);
}