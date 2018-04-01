#pragma once

#include "Mesh.h"

#include <SDL_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model(const std::string& path) {
		LoadModel(path);
	}

	~Model();

	void Draw(Shader shader);
private:
	std::vector<Mesh>    meshes;
	std::vector<Texture> loaded_Textures;

	std::string directory;

	void LoadModel(const std::string& path);

	void ProcessNode(aiNode* node, const aiScene* scene);

	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	void SetupVertices(Vertex& vertex, aiMesh* mesh, GLuint index);
	void SetupUVs(Vertex& vertex, aiMesh* mesh, GLuint index);
	void SetupTangents(Vertex& vertex, aiMesh* mesh, GLuint index);
	void SetupBitangents(Vertex& vertex, aiMesh* mesh, GLuint index);
	void SetupIndices(std::vector<GLuint>& indices, aiMesh* mesh);
	void SetupMaterialTexture(std::vector<Texture>& textures, aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> LoadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName);
	void CheckForLoadedTextures(bool& skip, std::vector<Texture>& textures, aiString str);

	void SetupTextureIfNotLoaded(bool skip, std::vector<Texture>& textures, aiString str, std::string typeName); // TODO: Change name

	GLuint LoadFromFile(std::string filePath, std::string directory);
};