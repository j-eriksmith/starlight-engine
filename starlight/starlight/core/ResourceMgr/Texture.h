#pragma once

struct Texture
{
	Texture(unsigned char* inData, int inWidth, int inHeight, int inNumChannels)
		: Data(inData),
		Width(inWidth),
		Height(inHeight),
		NumChannels(inNumChannels) {}

	unsigned char* Data;
	int Width, Height, NumChannels;
};
