#include "Model.h"

Model::~Model()
{
	meshes.clear();
	loaded_Textures.clear();
}

void Model::Draw(Shader shader)
{
	for (auto& m : meshes) {
		m.Draw(shader);
	}
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		SetupVertices(vertex, mesh, i);
		SetupUVs(vertex, mesh, i);
		SetupTangents(vertex, mesh, i);
		SetupBitangents(vertex, mesh, i);

		vertices.push_back(vertex);
	}

	SetupIndices(indices, mesh);
	SetupMaterialTexture(textures, mesh, scene);

	return Mesh(vertices, indices, textures);
}

void Model::SetupVertices(Vertex& vertex, aiMesh* mesh, GLuint index)
{
	glm::vec3 vector;
	vector.x = mesh->mVertices[index].x;
	vector.y = mesh->mVertices[index].y;
	vector.z = mesh->mVertices[index].z;
	vertex.Position = vector;
}

void Model::SetupUVs(Vertex& vertex, aiMesh* mesh, GLuint index)
{
	if (mesh->mTextureCoords[0]) {
		glm::vec2 vector;
		vector.x = mesh->mTextureCoords[0][index].x;
		vector.y = mesh->mTextureCoords[0][index].y;
		vertex.UV = vector;
	} else vertex.UV = glm::vec2(0.0f);
}

void Model::SetupTangents(Vertex& vertex, aiMesh* mesh, GLuint index)
{
	glm::vec3 vector;
	vector.x = mesh->mTangents[index].x;
	vector.y = mesh->mTangents[index].y;
	vector.z = mesh->mTangents[index].z;
	vertex.Tangent = vector;
}

void Model::SetupBitangents(Vertex& vertex, aiMesh* mesh, GLuint index)
{
	glm::vec3 vector;
	vector.x = mesh->mBitangents[index].x;
	vector.y = mesh->mBitangents[index].y;
	vector.z = mesh->mBitangents[index].z;
	vertex.Bitangent = vector;
}

void Model::SetupIndices(std::vector<GLuint>& indices, aiMesh* mesh)
{
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
}

void Model::SetupMaterialTexture(std::vector<Texture>& textures, aiMesh* mesh, const aiScene* scene)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture> diffuseMaps = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> normalMaps = LoadMaterialTexture(material, aiTextureType_NORMALS, "texture_normal");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> heightMaps = LoadMaterialTexture(material, aiTextureType_HEIGHT, "texture_height");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
}

std::vector<Texture> Model::LoadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		CheckForLoadedTextures(skip, textures, str);

		if (!skip) {
			SetupTextureIfNotLoaded(skip, textures, str, typeName);
		}
	}

	return textures;
}

void Model::CheckForLoadedTextures(bool& skip, std::vector<Texture>& textures, aiString str)
{
	for (GLuint j = 0; j < loaded_Textures.size(); j++) {
		if (std::strcmp(loaded_Textures[j].path.data(), str.C_Str()) == 0) {
			textures.push_back(loaded_Textures[j]);
			skip = true;
			break;
		}
	}
}

void Model::SetupTextureIfNotLoaded(bool skip, std::vector<Texture>& textures, aiString str, std::string typeName)
{
	if (!skip) {
		Texture texture;
		texture.id = LoadFromFile(str.C_Str(), this->directory);
		texture.type = typeName;
		texture.path = str.C_Str();

		textures.push_back(texture);
		loaded_Textures.push_back(texture);
	}
}

GLuint Model::LoadFromFile(std::string filePath, std::string directory)
{
	std::string fileName = filePath;
	fileName = directory + '/' + fileName;

	SDL_Surface* surface = IMG_Load(fileName.c_str());

	GLuint texID = 0;
	glGenTextures(1, &texID);

	int w = surface->w;
	int h = surface->h;
	int format = 0;

	const void* data = surface->pixels;

	if (surface != nullptr) {
		glBindTexture(GL_TEXTURE_2D, texID);

		surface->format->BytesPerPixel == 4 ? format = GL_RGBA : format = GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, nullptr);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		SDL_FreeSurface(surface);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return texID;
}

