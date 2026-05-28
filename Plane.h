#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

struct Plane {
	Vector3 normal;
	float distance;
};

Vector3 Perpendicular(const Vector3& vector);

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);