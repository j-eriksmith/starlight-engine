#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Vertex.h"


//! This class will store the vertex data for the default models to be used in DefaultModel.cpp.
class ShapeLoader
{
public:
	static enum class ShapeType
	{
		Cube,
		Sphere,
		Torus
	};
	static void CompleteDefaultShapeFileName(std::string& fileName, const std::string& dataType, const ShapeType& shapeType );
	// Load shape data from csv file and populate a vector of vertices with it
	static std::vector<Vertex> LoadVertexData(ShapeLoader::ShapeType shapeType);
	static std::vector<unsigned int> LoadIndexData(ShapeLoader::ShapeType shapeType);
	// Cache the cube Vertex and index data after the first retrieval to avoid wasting time
	// retrieving it for multiple cube generations.
	static std::vector<Vertex> cubeVertexData;
	static std::vector<unsigned int> cubeIndexData;
	static void ExtractFloatDataFromStream(std::stringstream& ss, std::vector<float>& out, int numItems);
	static void ExtractIntDataFromStream(std::stringstream& ss, std::vector<int>& out, int numItems);
	static void GetIndexDataFromFile(const std::string& fileName);
	static void GetVec3Data(std::stringstream& ss, glm::vec3& out);
	static void GetVec2Data(std::stringstream& ss, glm::vec2& out);
	static void GetVertexDataFromFile(const std::string& fileName);
};