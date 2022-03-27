#include "ResourceManager.h"

#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::unordered_map<std::string, Shader> ResourceManager::Shaders;
std::unordered_map<std::string, Texture> ResourceManager::Textures;


Shader ResourceManager::LoadShader(const std::string& name, const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file)
{
	Shaders[name] = LoadShaderFromFile(vertex_shader_file, fragment_shader_file, geometry_shader_file);
	return Shaders.at(name);
}

Shader ResourceManager::GetShader(const std::string& name) {
	return Shaders.at(name);
}

Texture ResourceManager::LoadTexture(const std::string& name, const char* file, bool alpha) {
	Textures[name] = LoadTextureFromFile(file, alpha);
	return Textures.at(name);
}

Texture ResourceManager::GetTexture(const std::string& name) {
	return Textures.at(name);
}

void ResourceManager::Clear() {
	Shaders.clear();
	Textures.clear();
}

Shader ResourceManager::LoadShaderFromFile(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vertex_shader_file);
        std::ifstream fragmentShaderFile(fragment_shader_file);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (geometry_shader_file != nullptr)
        {
            std::ifstream geometryShaderFile(geometry_shader_file);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cerr << "ERROR::SHADER: Failed to read shader files" << std::endl;
        throw std::runtime_error("ERROR::SHADER: Failed to read shader files");
    }
    // create shader object from source code
    Shader shader;
    shader.Compile(vertexCode.c_str(), fragmentCode.c_str(), geometry_shader_file != nullptr ? geometryCode.c_str() : nullptr);
    return shader;
}

Texture ResourceManager::LoadTextureFromFile(const char* file, bool alpha) {
    Texture texture;
    if (alpha)
    {
        texture.SetInternalFormat(GL_RGBA);
        texture.SetImageFormat(GL_RGBA);
    }
    // load image
	int width, height, num_of_channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(file, &width, &height, &num_of_channels, 0);
	if (!data) {
		std::cerr << "Failed to load texture from " << file << std::endl;
		throw std::runtime_error("Failed to load texture");
	}
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}
