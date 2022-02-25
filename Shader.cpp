#include "Shader.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

using namespace std::literals;

// GL ERROR HANDLING REQUIRED
Shader::Shader(const char* vertex_shader_location, const char* fragment_shader_location) {
	try {
		std::string vertex_shader_code = ReadFile(vertex_shader_location);
		std::string fragment_shader_code = ReadFile(fragment_shader_location);
		Compile(vertex_shader_code.c_str(), fragment_shader_code.c_str());
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		Clear();
		throw;
	}
}

std::string Shader::ReadFile(const char* flocation) {
	std::string content;
	std::ifstream fstream(flocation, std::ios::in);
	if (!fstream.is_open()) {
		throw std::runtime_error("Failed to read "s + std::string(flocation) + "! File doesnt exist."s);
	}
	std::string line = ""s;
	while (!fstream.eof()) {
		std::getline(fstream, line);
		content.append(line + "\n");
	}
	fstream.close();
	return content;
}

void Shader::Compile(const char* vertex_shader_code, const char* fragment_shader_code) {
	// GL ERROR HANDLING REQUIRED
	shader_id_ = glCreateProgram();
	if (shader_id_ == 0) {
		throw std::runtime_error("glCreateProgram() failed"s);
	}

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	if (vertex_shader == 0) {
		throw std::runtime_error("glCreateShader(GL_VERTEX_SHADER) failed"s);
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (vertex_shader == 0) {
		glDeleteShader(vertex_shader);
		throw std::runtime_error("glCreateShader(GL_FRAGMENT_SHADER) failed"s);
	}

	try {
		AddShader(vertex_shader, vertex_shader_code);
	}
	catch (const std::runtime_error& e) {
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		throw std::runtime_error("VERTEX SHADER "s + e.what());
	}

	try {
		AddShader(fragment_shader, fragment_shader_code);
	}
	catch (const std::runtime_error& e) {
		glDetachShader(shader_id_, vertex_shader);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		throw std::runtime_error("FRAGMENT SHADER "s + e.what());
	}

	glLinkProgram(shader_id_);

	GLint result = 0;
	GLchar info_log[1024] = { 0 };
	glGetProgramiv(shader_id_, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader_id_, sizeof(info_log), NULL, info_log);
		glDetachShader(shader_id_, vertex_shader);
		glDetachShader(shader_id_, fragment_shader);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		throw std::runtime_error("Error linking program: "s + std::string(info_log));
	}

	glDetachShader(shader_id_, vertex_shader);
	glDetachShader(shader_id_, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void Shader::AddShader(GLuint shader_id, const char* shader_code) {
	glShaderSource(shader_id, 1, &shader_code, NULL);
	glCompileShader(shader_id);

	GLint result = 0;
	GLchar info_log[1024] = { 0 };

	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader_id, sizeof(info_log), NULL, info_log);
		throw std::runtime_error("Error compiling the shader: "s + std::string(info_log));
	}

	glAttachShader(shader_id_, shader_id);
}

void Shader::Use() const {
	if (shader_id_ == 0) {
		throw std::runtime_error("Using empty shader program"s);
	}
	glUseProgram(shader_id_);
}

void Shader::SetInt(const char* varname, int value) {
	GLint location = glGetUniformLocation(shader_id_, varname);
	if (location == -1) {
		std::cerr << "Unable to find uniform (" << varname << ") location in shader : " << shader_id_ << std::endl;
		throw std::runtime_error("Unable to find uniform location");
	}
	glUniform1i(location, value);
}

void Shader::Clear() {
	if (shader_id_ != 0) {
		glDeleteProgram(shader_id_);
	}
}

Shader::~Shader() {
	Clear();
}
