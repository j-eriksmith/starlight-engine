#include "ModelLoadingSystem.h"
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>
#include <iostream>
#include "GLAD/include/glad.h"
#include "stb_image/stb_image.h"
#include "MemMgr.h"
#include "Debug.h"

void ModelLoadingSystem::Update(float deltaTime)
{
}

RenderableComponentPtr ModelLoadingSystem::LoadModel(const std::string& path)
{
		// Importer is an interface to the Assimp functionality
		//! @todo: Make an importer singleton object to avoid constructing this object every time we want to load a model.
		// NOTE the importer class is NOT thread safe, so you should construct a new one per-thread.
		Assimp::Importer import;
		// Import a file. Returns a ptr to the file's data, which is now a readonly attribute of import
		// For a list of all flags, see: http://sir-kimmi.de/assimp/lib_html/postprocess_8h.html#a64795260b95f5a4b3f3dc1be4f52e410
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "Error::ASSIMP::" << import.GetErrorString() << std::endl;
			return nullptr;
		}
		// Save directory path of the loaded model
		std::string directory = path.substr(0, path.find_last_of('/'));
		MeshVector meshes;
		BoundingBoxPrimitives bb;
		LoadedTextureMap textures_loaded;
		// Process all the nodes in the scene
		ProcessNode(scene->mRootNode, scene, bb, meshes, directory, textures_loaded);
		// Use MemMgr to alloc space for a new RenderableComponent
		return RenderableComponentPtr(new (MemMgr::Alloc(sizeof(RenderableComponent) + sizeof(MeshVector) * meshes.size(), MemMgr::AllocatorType::LevelData)) 
										   RenderableComponent(meshes, bb, true));
}

void ModelLoadingSystem::ProcessNode(aiNode* node, 
									 const aiScene* scene,
									 BoundingBoxPrimitives& bb,
									 MeshVector& meshes,
									 const std::string& directory,
									 LoadedTextureMap& textures_loaded)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh*& currentMesh = scene->mMeshes[node->mMeshes[i]];
		std::shared_ptr<Mesh> newMesh(ProcessMesh(currentMesh, scene, directory, textures_loaded));
		UpdateBoundingBoxValues(bb, currentMesh);
		meshes.push_back(newMesh);
	}
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene, bb, meshes, directory, textures_loaded);
	}
}

void ModelLoadingSystem::UpdateBoundingBoxValues(BoundingBoxPrimitives& bb,
												 aiMesh* mesh)
{
	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		float curX = mesh->mVertices[i].x;
		float curY = mesh->mVertices[i].y;
		float curZ = mesh->mVertices[i].z;
		bb.minX = std::min(bb.minX, curX);
		bb.maxX = std::max(bb.maxX, curX);
		bb.minY = std::min(bb.minY, curY);
		bb.maxY = std::max(bb.maxY, curY);
		bb.minZ = std::min(bb.minZ, curZ);
		bb.maxZ = std::max(bb.maxZ, curZ);
	}
}

// Summary:
// Consolidate assimp's vertex data (positions, normals, texCoords, indices) into our own Vertex objects
// and retrieve the mesh's texture data as well.
// @return a mesh object containing the consolidated vertex information
std::shared_ptr<Mesh> ModelLoadingSystem::ProcessMesh(aiMesh* mesh, 
													  const aiScene* scene,
													  const std::string& directory,
													  LoadedTextureMap& textures_loaded)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;

		glm::vec3 vec(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Position = vec;
		vec = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.Normal = vec;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 texCoords(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			vertex.TexCoords = texCoords;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace currentFace = mesh->mFaces[i];
		for (unsigned int j = 0; j < currentFace.mNumIndices; ++j)
		{
			indices.push_back(currentFace.mIndices[j]);
		}
	}
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<std::shared_ptr<Texture>> diffuseMaps = LoadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse", directory, textures_loaded);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<std::shared_ptr<Texture>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", directory, textures_loaded);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	std::shared_ptr<Mesh> newMesh(new (MemMgr::Alloc(sizeof(Mesh), MemMgr::AllocatorType::LevelData)) Mesh(vertices, indices, textures));
	return newMesh;
}

std::vector<std::shared_ptr<Texture>> ModelLoadingSystem::LoadMaterialTextures(aiMaterial* mat, 
															  aiTextureType type, 
															  const std::string& typeName,
															  const std::string& directory,
															  LoadedTextureMap& textures_loaded)
{
	std::vector<std::shared_ptr<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		// Holds the path of the current texture to load.
		aiString str;
		mat->GetTexture(type, i, &str);
		Log("texture path: " << str.C_Str());
		Log("directory: " << directory);
		bool skip = false;
		if (textures_loaded.find(str.C_Str()) != textures_loaded.end())
		{
			textures.push_back(textures_loaded[str.C_Str()]);
			skip = true;
			break;
		}
		if (!skip)
		{
			std::string fileName = directory + "/" + str.C_Str();
			std::shared_ptr<Texture>texture(std::shared_ptr<Texture>(new Texture(fileName, typeName)));
			textures.push_back(texture);
			textures_loaded[str.C_Str()] = texture;
		}
	}
	return textures;
}