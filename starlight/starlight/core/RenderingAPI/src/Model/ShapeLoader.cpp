#include "ShapeLoader.h"
#include <fstream>

std::vector<Vertex> ShapeLoader::cubeVertexData;
std::vector<unsigned int> ShapeLoader::cubeIndexData;

void ShapeLoader::CompleteDefaultShapeFileName(std::string& fileName, const std::string& dataType, const ShapeType& shapeType)
{
	switch (shapeType)
	{
		case ShapeType::Cube:
		{
			fileName += "cube_" + dataType + ".csv";
			break;
		}
		case ShapeType::Sphere:
		{
			fileName += "sphere_" + dataType + ".csv";
			break;
		}
		case ShapeType::Torus:
		{
			fileName += "torus_" + dataType + ".csv";
			break;
		}
	}
}

void ShapeLoader::ExtractFloatDataFromStream(std::stringstream& ss, std::vector<float>& out, int numItems)
{
	std::string line;
	for (int i = 0; i < numItems; ++i)
	{
		std::getline(ss, line, ',');
		out.push_back(std::stof(line));
	}
}

void ShapeLoader::ExtractIntDataFromStream(std::stringstream& ss, std::vector<int>& out, int numItems)
{
	std::string line;
	for (int i = 0; i < numItems; ++i)
	{
		std::getline(ss, line, ',');
		out.push_back(std::stoi(line));
	}
}

void ShapeLoader::GetVec3Data(std::stringstream& ss, glm::vec3& out)
{
	std::vector<float> vData;
	ExtractFloatDataFromStream(ss, vData, 3);
	out.x = vData[0];
	out.y = vData[1];
	out.z = vData[2];
}

void ShapeLoader::GetVec2Data(std::stringstream& ss, glm::vec2& out)
{
	std::vector<float> vData;
	ExtractFloatDataFromStream(ss, vData, 2);
	out.x = vData[0];
	out.y = vData[1];
}

void ShapeLoader::GetIndexDataFromFile(const std::string& fileName)
{
	std::string line;
	std::ifstream iData(fileName);
	while (std::getline(iData, line))
	{
		std::stringstream ss(line);
		std::vector<int> indices;
		ExtractIntDataFromStream(ss, indices, 3);
		cubeIndexData.insert(cubeIndexData.end(), indices.begin(), indices.end());
	}
	for (unsigned int data : cubeIndexData)
	{
		std::cout << data << " ";
	}
	std::cout << std::endl;
}

void ShapeLoader::GetVertexDataFromFile(const std::string& fileName)
{
	std::string line;
	std::ifstream vData(fileName); 
	std::cout << "file " << fileName << " is open: " << vData.is_open() << std::endl;
	while (getline(vData, line))
	{
		Vertex v;
		std::stringstream ss(line);
		GetVec3Data(ss, v.Position);
		GetVec3Data(ss, v.Normal);
		GetVec2Data(ss, v.TexCoords);
		cubeVertexData.push_back(v);
	}
	vData.close();
}

std::vector<Vertex> ShapeLoader::LoadVertexData(ShapeLoader::ShapeType shapeType)
{
	std::string fileName("core/RenderingAPI/src/Model/default_shapes/");
	CompleteDefaultShapeFileName(fileName, "vertices", shapeType);
	GetVertexDataFromFile(fileName);
	return cubeVertexData;
}

std::vector<unsigned int> ShapeLoader::LoadIndexData(ShapeLoader::ShapeType shapeType)
{
	std::string fileName("core/RenderingAPI/src/Model/default_shapes/");
	CompleteDefaultShapeFileName(fileName, "indices", shapeType);
	GetIndexDataFromFile(fileName);
	return cubeIndexData;
}