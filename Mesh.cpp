#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

Mesh::~Mesh()
{
	//glDeleteVertexArrays(1, &vao);
	//glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ebo);

	vertices.clear();
	indices.clear();
	textures.clear();
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	AddDataToShader(GL_ARRAY_BUFFER, vbo, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	AddDataToShader(GL_ELEMENT_ARRAY_BUFFER, ebo, indices.size() * sizeof(GL_UNSIGNED_INT), indices.data(), GL_STATIC_DRAW);

	SetupVertexAttributes(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	SetupVertexAttributes(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	SetupVertexAttributes(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
	SetupVertexAttributes(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	SetupVertexAttributes(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

void Mesh::SetupVertexAttributes(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* data)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, data);
}

void Mesh::AddDataToShader(GLenum target, GLuint buffer, GLsizeiptr size, const GLvoid* data, GLenum usage)
{
	glBindBuffer(target, buffer);
	glBufferData(target, size, data, usage);
	glBufferSubData(target, 0, size, data);
}

void Mesh::Draw(Shader shader)
{
	for (GLuint i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		
		std::string name = textures[i].type;

		glUniform1f(shader.GetUniformLocation(name + DefineNumber(name)), (GLfloat)i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(vao);
	//glDrawElementsInstancedBaseInstance(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0, 1, 0);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

std::string Mesh::DefineNumber(std::string name)
{
	GLuint diffuseNr = 0;
	GLuint specularNr = 0;
	GLuint normalNr = 0;
	GLuint heightNr = 0;

	std::string number;

	if (name.find("texture_diffuse") != std::string::npos) {
		number = std::to_string(diffuseNr++);
	} else if (name.find("texture_specural") != std::string::npos) {
		number = std::to_string(specularNr++);
	} else if (name.find("texture_normal") != std::string::npos) {
		number = std::to_string(normalNr++);
	} else if (name.find("texture_height") != std::string::npos) {
		number = std::to_string(heightNr++);
	}

	return number;
}