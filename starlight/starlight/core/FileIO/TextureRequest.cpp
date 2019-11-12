#pragma once
#include "FileIO.h"

#include "stb_image/stb_image.h"

void TextureRequest::Fulfill()
{
	int Width, Height, NumChannels;
	unsigned char* Data = stbi_load(FileName.c_str(), &Width, &Height, &NumChannels, 0);

	// TODO(jake): Take from static allocator
	this->LoadedTexture = new Texture(Data, Width, Height, NumChannels);
}