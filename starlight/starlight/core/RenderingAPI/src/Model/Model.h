#pragma once
#include "Shader.h"

class Model
{
public:
	virtual void Draw(Shader& shader) = 0;
};