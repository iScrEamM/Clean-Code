#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glew.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Shader
{
public:
	Shader();
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();

	void CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);

	GLint GetUniformLocation(const std::string &uniformName) const;

	void useProgram();

	GLuint getShaderProgram() const;

	void UniformMatrix4fv(glm::mat4 matrix, std::string name);
private:
	GLuint shaderProgram;

	std::string LoadFromFile(const std::string &filePath);
	void ShaderCompiledError(GLuint shaderID);
	void ShaderLinkedError(GLuint vertexShaderID, GLuint fragmentShaderID);
};

class ShaderManager
{
public:
	ShaderManager() {}
	~ShaderManager() { shader.clear(); }

	Shader &operator[](const std::string &shaderName) {
		return shader[shaderName];
	}

private:
	std::map<std::string, Shader> shader;
};
#endif SHADER_