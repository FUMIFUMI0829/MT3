#pragma once
#include "MT3.h"

struct Sphere {
	Vector3 center;
	float radius;
};
bool IsCollision(const Sphere& s1, const Sphere& s2);

void DrawSphere(const Sphere& sphere, const Matrix4x4 viewProjectionMatrix, const Matrix4x4 viewportMatrix, unsigned int color);

