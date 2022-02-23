#include "ShaderProgram.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

using namespace std::literals;

// GL ERROR HANDLING REQUIRED

void ShaderProgram::CreateFromStrings(const char* vertex_shader_code, const char* fragment_shader_code) {
	try {
		Compile(vertex_shader_code, fragment_shader_code);
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		Clear();
		throw;
	}
}

void ShaderProgram::CreateFromFiles(const char* vertex_shader_location, const char* fragment_shader_location) {
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

std::string ShaderProgram::ReadFile(const char* flocation) {
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

void ShaderProgram::Compile(const char* vertex_shader_code, const char* fragment_shader_code) {
	// GL ERROR HANDLING REQUIRED
	shader_program_object_ = glCreateProgram();
	if (shader_program_object_ == 0) {
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
		glDetachShader(shader_program_object_, vertex_shader);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		throw std::runtime_error("FRAGMENT SHADER "s + e.what());
	}

	glLinkProgram(shader_program_object_);

	GLint result = 0;
	GLchar info_log[1024] = { 0 };
	glGetProgramiv(shader_program_object_, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader_program_object_, sizeof(info_log), NULL, info_log);
		glDetachShader(shader_program_object_, vertex_shader);
		glDetachShader(shader_program_object_, fragment_shader);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		throw std::runtime_error("Error linking program: "s + std::string(info_log));
	}

	glDetachShader(shader_program_object_, vertex_shader);
	glDetachShader(shader_program_object_, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void ShaderProgram::AddShader(GLuint shader_id, const char* shader_code) {
	const GLchar* the_code[1];
	the_code[0] = shader_code;

	GLint code_length[1];
	code_length[0] = std::strlen(shader_code);

	glShaderSource(shader_id, 1, the_code, code_length);
	glCompileShader(shader_id);

	GLint result = 0;
	GLchar info_log[1024] = { 0 };

	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader_id, sizeof(info_log), NULL, info_log);
		throw std::runtime_error("Error compiling the shader: "s + std::string(info_log));
	}

	glAttachShader(shader_program_object_, shader_id);
}

void ShaderProgram::Use() const {
	if (shader_program_object_ == 0) {
		throw std::runtime_error("Using empty shader program"s);
	}
	glUseProgram(shader_program_object_);
}

void ShaderProgram::Clear() {
	if (shader_program_object_ != 0) {
		glDeleteProgram(shader_program_object_);
	}
}

ShaderProgram::~ShaderProgram() {
	Clear();
}
