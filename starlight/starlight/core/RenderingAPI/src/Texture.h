#pragma once
#include "Renderer.h"

class Texture
{

public:

	Texture(const std::string& path, const std::string& textureType = "diffuse");
	Texture();
	~Texture();

	// OpenGL allows you to bind multiple textures at once. The number of texture 'slots' you have depends on your
	// GPU's implementation of OpenGL
	void Bind(unsigned int slot = 0) const;

	unsigned int GetColorFormat(int bytesPerPixel);

	void Unbind() const;

	unsigned int id;
	std::string path;
	std::string type;

	
};