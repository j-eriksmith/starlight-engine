#include "BoundingBox.h"
constexpr double DOUBLE_MIN = std::numeric_limits<double>::min();

BoundingBox::BoundingBox(Vector3 c, double x, double y, double z)
	:center(c), radiusX(x), radiusY(y), radiusZ(z)
{}

BoundingBox::BoundingBox()
	:center(Vector3()), radiusX(DOUBLE_MIN), radiusY(DOUBLE_MIN), radiusZ(DOUBLE_MIN)
{}