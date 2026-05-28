#pragma once
#include "MT3.h"

struct Plane {
	Vector3 normal;
	float distance;
};

void DrawPlane(
	const Plane& plane,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	unsigned int color);