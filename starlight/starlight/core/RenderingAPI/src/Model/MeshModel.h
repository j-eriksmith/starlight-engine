#pragma once
#include "BoundingBox.h"
#include "Shader.h"
#include "Model.h"
#include "Mesh.h"
#include <vector>
#include <string>
#include <assimp/include/assimp/scene.h>
#include <map>
#include "Texture.h"
#include "vec3.h"

class MeshModel : public Model
{
public:
	MeshModel(const char* path);
	void Draw(Shader& shader);
	void Unbind();
private:
	BoundingBox boundingBox;
	std::map<std::string, Texture> textures_loaded;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::string directory;
	
	BoundingBox CreateBoundingBox(const BoundingBoxPrimitives& bb);
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	void MeshModel::UpdateBoundingBoxValues(BoundingBoxPrimitives& bb, aiMesh* mesh);
};