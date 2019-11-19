#include "Texture.h"
#include "stb_image/stb_image.h"
#include <iostream>

Texture::Texture(const std::string& filePath, const std::string& textureType)
	:id(0), path(filePath), type(textureType)
{
	// OpenGL usually expects our image's first pixel to start at the bottom right corner, but
	// in reality it starts at the top left. This fn call will fix this issue.
	stbi_set_flip_vertically_on_load(1);
	int width, height, bytesPerPixel;
	std::cout << "Path: " << path.c_str() << std::endl;
	unsigned char* localBuffer = stbi_load(path.c_str(), &width, &height, &bytesPerPixel, 4);
	if (localBuffer)
	{
		GLCall(glGenTextures(1, &id));
		GLCall(glBindTexture(GL_TEXTURE_2D, id));

		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		// setting up our texture parameter
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		//send opengl our texture data
		unsigned int colorFormat = GetColorFormat(bytesPerPixel);
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, localBuffer));
		// unbind our data
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	else
	{
		std::cout << "Texture::Texture -- Failed to load texture data for file " << filePath << std::endl;
	}

	stbi_image_free(localBuffer);
		
}

Texture::Texture()
	:id(0), path(""), type("diffuse")
{
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &id));
}

void Texture::Bind(unsigned int slot) const
{
	// make our active texture slot x so that next time I try to bind / unbind a texture,
	// slot x is effected. Our 'slot' parameter provides an offset from the base GL_TEXTURE0
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));
}

unsigned int Texture::GetColorFormat(int bytesPerPixel)
{
	GLenum format = GL_RGBA;
	if (bytesPerPixel == 1)
		format = GL_RED;
	else if (bytesPerPixel == 3)
		format = GL_RGB;
	return format;
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}