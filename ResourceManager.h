#pragma once

#include <unordered_map>
#include <string>

#include <GL/glew.h>

#include "Texture.h"
#include "Shader.h"

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResourceManager {
public:
	static std::unordered_map<std::string, Shader> Shaders;
	static std::unordered_map<std::string, Texture> Textures;

	static Shader LoadShader(const std::string& name, const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file = nullptr);
	static Shader GetShader(const std::string& name);

	static Texture LoadTexture(const std::string& name, const char* file, bool alpha);
	static Texture GetTexture(const std::string& name);

	static void Clear();
private:
	ResourceManager() {}
	
	static Shader LoadShaderFromFile(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file = nullptr);

	static Texture LoadTextureFromFile(const char* file, bool alpha);
};

