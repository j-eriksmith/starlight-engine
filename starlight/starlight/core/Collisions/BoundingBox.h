#pragma once
#include <limits>
#include "vec3.h"
struct BoundingBox
{

	BoundingBox(Vector3 c, double x, double y, double z);
	BoundingBox();
	Vector3 center;
	double radiusX;
	double radiusY;
	double radiusZ;
};

struct BoundingBoxPrimitives
{
	double maxX;
	double maxY;
	double maxZ;
	double minX;
	double minY;
	double minZ;
};