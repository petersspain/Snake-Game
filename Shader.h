#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Shader {
public:
	Shader() = default;
	~Shader();

	Shader& Use();

	void Compile(const char* vertex_shader_code, const char* fragment_shader_code, const char* geometry_shader_code = nullptr);

	GLuint GetShaderID() const { return shader_id_; }

	void SetInt(const char* varname, int value, bool use_shader = false);
	void SetFloat(const char* varname, float value, bool use_shader = false);
	void SetVector2f(const char* varname, float x, float y, bool use_shader = false);
	void SetVector2f(const char* varname, const glm::vec2& value, bool use_shader = false);
	void SetVector3f(const char* varname, float x, float y, float z, bool use_shader = false);
	void SetVector3f(const char* varname, const glm::vec3& value, bool use_shader = false);
	void SetVector4f(const char* varname, float x, float y, float z, float w, bool use_shader = false);
	void SetVector4f(const char* varname, const glm::vec4& value, bool use_shader = false);
	void SetMatrix4(const char* varname, const glm::mat4& matrix, bool transpose = GL_FALSE, bool use_shader = false);
private:
	void AddShader(GLuint shader_id, const char* shader_code);
private:
	size_t shader_id_ = 0;
};