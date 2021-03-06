#pragma once
#include "System.h"
#include "RenderableComponent.h"
#include <memory>
#include <assimp/include/assimp/scene.h>
#include "Mesh.h"
#include <string>
#include <Texture.h>
#include "vec3.h"
#include <vector>


using MeshVector = std::vector<std::shared_ptr<Mesh>>;
using LoadedTextureMap = std::map<std::string, std::shared_ptr<Texture>>;
class ModelLoadingSystem : public System<RenderableComponent>
{
	using BaseType = System<RenderableComponent>;
public:
	ModelLoadingSystem()
		: BaseType(nullptr) {}
	ModelLoadingSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;

	static RenderableComponentPtr LoadModel(const std::string& path);

	static void ProcessNode(aiNode* node, 
					 const aiScene* scene, 
					 BoundingBoxPrimitives& bb, 
					 MeshVector& meshes,
					 const std::string& directory,
					 LoadedTextureMap& textures_loaded);

	static std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, 
									  const aiScene* scene,
									  const std::string& directory,
									  LoadedTextureMap& texture_loaded);

	static std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, 
											  aiTextureType type, 
											  const std::string& typeName,
											  const std::string& directory,
											  LoadedTextureMap& textures_loaded);

	static void UpdateBoundingBoxValues(BoundingBoxPrimitives& bb, 
								 aiMesh* mesh);

};