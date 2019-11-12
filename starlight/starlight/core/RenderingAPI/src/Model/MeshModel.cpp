#include "MeshModel.h"
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>
#include <iostream>
#include "GLAD/include/glad.h"
#include "stb_image/stb_image.h"
#include <memory>

MeshModel::MeshModel(const char* path)
{
	loadModel(path);
}

void MeshModel::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i]->Draw(shader);
	}
}

void MeshModel::loadModel(std::string path)
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
		return;
	}
	// Save directory path of the loaded model
	directory = path.substr(0, path.find_last_of('/'));
	// Process all the nodes in the scene
	processNode(scene->mRootNode, scene);
}

void MeshModel::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		//std::cout << "Processing mesh number " << i << std::endl;
		std::shared_ptr<Mesh> newMesh(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
		meshes.push_back(newMesh);
	}
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}
// Summary:
// Consolidate assimp's vertex data (positions, normals, texCoords, indices) into our own Vertex objects
// and retrieve the mesh's texture data as well.
// @return a mesh object containing the consolidated vertex information
std::shared_ptr<Mesh> MeshModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

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
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	std::shared_ptr<Mesh> newMesh(new Mesh(vertices, indices, textures));
	return newMesh;
}
//
std::vector<Texture> MeshModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		// Holds the path of the current texture to load.
		aiString str;
		mat->GetTexture(type, i, &str);
		std::cout << "texture path: " << str.C_Str() << std::endl;
		std::cout << "directory: " << directory << std::endl;
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
			Texture texture(fileName, typeName);
			textures.push_back(texture);
			textures_loaded[str.C_Str()] = texture;
		}
	}
	return textures;
}

