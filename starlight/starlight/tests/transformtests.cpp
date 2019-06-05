#include "../core/math/transform.h"
#include "../core/math/utils.h"
#include <cassert>

void Transform::RunTests()
{
	Vector3 Zeros;
	Matrix3 Identity;
	Transform T(Identity, Zeros);

	// Translation 
	T = T.Translate(Vector3(5.0f, 5.0f, 5.0f));
	Transform Goal(Identity, Vector3(5.0f, 5.0f, 5.0f));
	assert(T == Goal);
	
	// Scaling
	T = T.Scale(Vector3(2.0f, 2.0f, 2.0f));
	Goal = Transform(Identity * 2.0f, Vector3(5.0f, 5.0f, 5.0f));
	assert(T == Goal);

	// Rotation - Verified via Wolfram-Alpha
	T = Transform(Identity, Zeros);
	T = T.Rotate(Vector3(1.0f, 0.0f, 0.0f), Utils::DegToRad(30.0f)); 
	Matrix3 Rot_Goal(Vector3(1.0f, 0.0f, 0.0f),
					Vector3(0.0f, Utils::Sqrt(3.0f) / 2.0f, -1.0 / 2.0f),
					Vector3(0.0f, 1.0f / 2.0f, Utils::Sqrt(3.0f) / 2.0f));
	Goal = Transform(Rot_Goal, Zeros);
	assert(T == Goal);
}