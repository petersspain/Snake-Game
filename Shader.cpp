#include "Shader.h"

#include <stdexcept>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

using namespace std::literals;

Shader::~Shader() {
	if (shader_id_ != 0) {
		glDeleteProgram(shader_id_);
	}
}

//std::string Shader::ReadFile(const char* flocation) {
//	std::string content;
//	std::ifstream fstream(flocation, std::ios::in);
//	if (!fstream.is_open()) {
//		throw std::runtime_error("Failed to read "s + std::string(flocation) + "! File doesnt exist."s);
//	}
//	std::string line = ""s;
//	while (!fstream.eof()) {
//		std::getline(fstream, line);
//		content.append(line + "\n");
//	}
//	fstream.close();
//	return content;
//}

void Shader::Compile(const char* vertex_shader_code, const char* fragment_shader_code, const char* geometry_shader_code) {
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

	GLuint geometry_shader = 0;
	if (geometry_shader_code != nullptr) {
		geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
		if (geometry_shader == 0) {
			glDeleteShader(geometry_shader);
			throw std::runtime_error("glCreateShader(GL_GEOMETRY_SHADER) failed"s);
		}
		try {
			AddShader(geometry_shader, geometry_shader_code);
		}
		catch (const std::runtime_error& e) {
			glDetachShader(shader_id_, vertex_shader);
			glDetachShader(shader_id_, fragment_shader);
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
			glDeleteShader(geometry_shader);
			throw std::runtime_error("FRAGMENT SHADER "s + e.what());
		}
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
		if (geometry_shader_code != nullptr) {
			glDetachShader(shader_id_, geometry_shader);
			glDeleteShader(geometry_shader);
		}
		throw std::runtime_error("Error linking program: "s + std::string(info_log));
	}

	glDetachShader(shader_id_, vertex_shader);
	glDetachShader(shader_id_, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	if (geometry_shader_code != nullptr) {
		glDetachShader(shader_id_, geometry_shader);
		glDeleteShader(geometry_shader);
	}
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

Shader& Shader::Use() {
	if (shader_id_ == 0) {
		throw std::runtime_error("Using empty shader program"s);
	}
	glUseProgram(shader_id_);
	return *this;
}

// NOTE: SetInt and SetFloat completely the same, the only deference is a type => may be a template (template<typename T> void Set(const char* varname, T value);) ????? glUniform1f, 1i, 3f .....
void Shader::SetInt(const char* varname, int value, bool use_shader) {
	if (use_shader) {
		Use();
	}
	GLint location = glGetUniformLocation(shader_id_, varname);
	if (location == -1) {
		std::cerr << "Unable to find uniform (" << varname << ") location in shader : " << shader_id_ << std::endl;
		throw std::runtime_error("Unable to find uniform location");
	}
	glUniform1i(location, value);
}

void Shader::SetFloat(const char* varname, float value, bool use_shader) {
	if (use_shader) {
		Use();
	}
	GLint location = glGetUniformLocation(shader_id_, varname);
	if (location == -1) {
		std::cerr << "Unable to find uniform (" << varname << ") location in shader : " << shader_id_ << std::endl;
		throw std::runtime_error("Unable to find uniform location");
	}
	glUniform1f(location, value);
}

void Shader::SetVector2f(const char* varname, float x, float y, bool use_shader) {
	if (use_shader) {
		Use();
	}
	GLint location = glGetUniformLocation(shader_id_, varname);
	if (location == -1) {
		std::cerr << "Unable to find uniform (" << varname << ") location in shader : " << shader_id_ << std::endl;
		throw std::runtime_error("Unable to find uniform location");
	}
	glUniform2f(location, x, y);
}

void Shader::SetVector2f(const char* varname, const glm::vec2& value, bool use_shader) {
	if (use_shader) {
		Use();
	}
	GLint location = glGetUniformLocation(shader_id_, varname);
	if (location == -1) {
		std::cerr << "Unable to find uniform (" << varname << ") location in shader : " << shader_id_ << std::endl;
		throw std::runtime_error("Unable to find uniform location");
	}
	glUniform2f(location, value.x, value.y);
}

void Shader::SetVector3f(const char* varname, float x, float y, float z, bool use_shader) {
	if (use_shader) {
		Use();
	}
	GLint location = glGetUniformLocation(shader_id_, varname);
	if (location == -1) {
		std::cerr << "Unable to find uniform (" << varname << ") location in shader : " << shader_id_ << std::endl;
		throw std::runtime_error("Unable to find uniform location");
	}
	glUniform3f(location, x, y, z);
}

void Shader::SetVector3f(const char* varname, const glm::vec3& value, bool use_shader) {
	if (use_shader) {
		Use();
	}
	GLint location = glGetUniformLocation(shader_id_, varname);
	if (location == -1) {
		std::cerr << "Unable to find uniform (" << varname << ") location in shader : " << shader_id_ << std::endl;
		throw std::runtime_error("Unable to find uniform location");
	}
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetVector4f(const char* varname, float x, float y, float z, float w, bool use_shader) {
	if (use_shader) {
		Use();
	}
	GLint location = glGetUniformLocation(shader_id_, varname);
	if (location == -1) {
		std::cerr << "Unable to find uniform (" << varname << ") location in shader : " << shader_id_ << std::endl;
		throw std::runtime_error("Unable to find uniform location");
	}
	glUniform4f(location, x, y, z, w);
}

void Shader::SetVector4f(const char* varname, const glm::vec4& value, bool use_shader) {
	if (use_shader) {
		Use();
	}
	GLint location = glGetUniformLocation(shader_id_, varname);
	if (location == -1) {
		std::cerr << "Unable to find uniform (" << varname << ") location in shader : " << shader_id_ << std::endl;
		throw std::runtime_error("Unable to find uniform location");
	}
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const char* varname, const glm::mat4& matrix, bool transpose, bool use_shader) {
	if (use_shader) {
		Use();
	}
	GLint location = glGetUniformLocation(shader_id_, varname);
	if (location == -1) {
		std::cerr << "Unable to find uniform (" << varname << ") location in shader : " << shader_id_ << std::endl;
		throw std::runtime_error("Unable to find uniform location");
	}
	glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(matrix));
}
