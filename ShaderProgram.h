#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ShaderProgram {
public:
	void CreateFromStrings(const char* vertex_shader_code, const char* fragment_shader_code);
	void CreateFromFiles(const char* vertex_shader_location, const char* fragment_shader_location);

	void Use() const;

	~ShaderProgram();
private:
	void Clear();

	void Compile(const char* vertex_shader_code, const char* fragment_shader_code);
	void AddShader(GLuint shader_id, const char* shader_code);

	std::string ReadFile(const char* flocation);
private:
	GLuint shader_program_object_ = 0;
};