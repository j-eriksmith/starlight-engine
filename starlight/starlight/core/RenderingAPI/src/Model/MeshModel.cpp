#include "MeshModel.h"
#include <algorithm>
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>
#include <iostream>
#include "GLAD/include/glad.h"
#include "stb_image/stb_image.h"
#include <memory>

// Populate an array that holds the position coordinates that OpenGL will use to render the MeshModel's AABB
std::vector<float> CreateVertexData(const BoundingBoxPrimitives& bb)
{
	return std::vector<float> {
		bb.minX, bb.maxY, bb.minZ, // top back left 0
		bb.minX, bb.maxY, bb.maxZ, // top front left 1
		bb.minX, bb.minY, bb.maxZ, // bottom front left 2
		bb.minX, bb.minY, bb.minZ, // bottom back left 3
		bb.maxX, bb.maxY, bb.minZ, // top back right 4
		bb.maxX, bb.maxY, bb.maxZ, // top front right 5
		bb.maxX, bb.minY, bb.maxZ, // bottom front right 6
		bb.maxX, bb.minY, bb.minZ // bottom back right 7
	};
}

std::vector<unsigned int> CreateIndexData()
{
	return std::vector<unsigned int>{
		// Left Face
			0, 2, 3,
			2, 0, 1,
			// Front Face
			1, 6, 2,
			6, 1, 5,
			// Right Face
			5, 7, 6,
			7, 5, 4,
			// Back Face
			4, 3, 7,
			3, 4, 0,
			// Top Face
			0, 5, 1,
			5, 0, 4,
			// Bottom Face
			2, 7, 3,
			7, 2, 6
	};
}

MeshModel::MeshModel(const char* path)
	:boundingBox(nullptr)
{
	loadModel(path);
}

std::shared_ptr<BoundingBox> MeshModel::CreateBoundingBox(const BoundingBoxPrimitives& bb)
{
	float midX = (bb.maxX - bb.minX) / 2;
	float midY = (bb.maxY - bb.minY) / 2;
	float midZ = (bb.maxZ - bb.minZ) / 2;
	Vector3 centerCoords(bb.minX + midX, bb.minY + midY, bb.minZ + midZ);
	std::vector<float> vertexData(CreateVertexData(bb));
	std::vector<unsigned int> indexData(CreateIndexData());
	return std::shared_ptr<BoundingBox>(new BoundingBox(centerCoords,
		   midX, midY, midZ, vertexData, indexData));
	// @TODO: store vertex array data for bounding box in the newly constructed bounding box object. 
	// Will need to create a vertex buffer layout object and pass in the vertex data.
}

void MeshModel::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i]->Draw(shader);
	}
	//boundingBox->Draw(shader);
}

std::shared_ptr<BoundingBox> MeshModel::GetBoundingBox()
{
	return boundingBox;
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
	boundingBox = CreateBoundingBox(bb);
}

void MeshModel::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh*& currentMesh = scene->mMeshes[node->mMeshes[i]];
		std::shared_ptr<Mesh> newMesh(processMesh(currentMesh, scene));
		UpdateBoundingBoxValues(bb, currentMesh);
		meshes.push_back(newMesh);
	}
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}

void MeshModel::UpdateBoundingBoxValues(BoundingBoxPrimitives& bb, aiMesh* mesh)
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

