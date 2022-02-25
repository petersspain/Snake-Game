#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader {
public:
	explicit Shader(const char* vertex_shader_location, const char* fragment_shader_location);

	void Use() const;

	void SetInt(const char* varname, int value);

	GLuint GetShaderId() const { return shader_id_; }

	~Shader();
private:
	void Clear();

	void Compile(const char* vertex_shader_code, const char* fragment_shader_code);
	void AddShader(GLuint shader_id, const char* shader_code);

	std::string ReadFile(const char* flocation);
private:
	GLuint shader_id_ = 0;
};