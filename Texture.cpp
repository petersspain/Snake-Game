#include "Texture.h"

Texture::Texture() {
	glGenTextures(1, &texture_id_);
}

void Texture::Generate(size_t width, size_t height, const unsigned char* data) {
	width_ = width;
	height_ = height;

	glBindTexture(GL_TEXTURE_2D, texture_id_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max_);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format_, width_, height_, 0, image_format_, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	//stbi_image_free(data);
}

void Texture::Bind() const {
	glBindTexture(GL_TEXTURE_2D, texture_id_);
}

//void Texture::ChangeTextureWrapTo(GLint param) {
//	glActiveTexture(texture_unit_);
//	glBindTexture(GL_TEXTURE_2D, texture_id_);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
//}
//
//void Texture::ChangeTextureMinFilterTo(GLint param) {
//	glActiveTexture(texture_unit_);
//	glBindTexture(GL_TEXTURE_2D, texture_id_);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
//}
//
//void Texture::ChangeTextureMagFilterTo(GLint param) {
//	glActiveTexture(texture_unit_);
//	glBindTexture(GL_TEXTURE_2D, texture_id_);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
//}

Texture::~Texture() {
	glDeleteTextures(1, &texture_id_);
}