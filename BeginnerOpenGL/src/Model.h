#pragma once

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include "Texture.h"
#include "Mesh.h"

class Model
{
public:
	std::vector<Texture*> textures_loaded;
	std::vector<Mesh*> meshes;
	std::string directory;

	Model(std::string const& path);

	void Draw(Shader& shader);

private:
	void LoadModel(std::string const& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};