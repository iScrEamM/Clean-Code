#pragma once

#include <glew.h>
#include <vector>
#include <glm.hpp>
#include <iostream>

#include "Shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;

	glm::vec2 UV;

	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh 
{
public:
	Mesh(std::vector<Vertex> vertices,
		 std::vector<GLuint> indices,
		 std::vector<Texture> textures);
	~Mesh();

	void Draw(Shader shader);
private:
	void SetupMesh();
	void SetupVertexAttributes(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* data);
	void AddDataToShader(GLenum target, GLuint buffer, GLsizeiptr size, const GLvoid* data, GLenum usage);

	std::string DefineNumber(std::string name);

	std::vector<Vertex>  vertices;
	std::vector<GLuint>  indices;
	std::vector<Texture> textures;

	GLuint vbo = 0;
	GLuint vao = 0;
	GLuint ebo = 0;

};