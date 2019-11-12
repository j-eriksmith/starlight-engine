#pragma once
#include "Shader.h"
#include "Model.h"
#include "Mesh.h"
#include <vector>
#include <string>
#include <assimp/include/assimp/scene.h>
#include <map>
#include "Texture.h"

class MeshModel : public Model
{
public:
	MeshModel(const char* path);
	void Draw(Shader& shader);
	void Unbind();
private:
	std::map<std::string, Texture> textures_loaded;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::string directory;
	
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};