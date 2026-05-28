#include "Sphere.h"
#include <cmath>
#include <numbers>
#include <Novice.h>

bool IsCollision(const Sphere& s1, const Sphere& s2) {

	// 中心点同士の差分
	Vector3 diff{
		s2.center.x - s1.center.x,
		s2.center.y - s1.center.y,
		s2.center.z - s1.center.z
	};

	// 距離の二乗
	float distanceSquared =
		diff.x * diff.x +
		diff.y * diff.y +
		diff.z * diff.z;

	// 半径の合計
	float radius = s1.radius + s2.radius;

	// 半径の合計の二乗
	float radiusSquared = radius * radius;

	// 衝突判定
	if (distanceSquared <= radiusSquared) {
		return true;
	}

	return false;
}

void DrawSphere(const Sphere& sphere, const Matrix4x4 viewProjectionMatrix, const Matrix4x4 viewportMatrix, unsigned int color) {
	const int kSubdivision = 16;

	const float kLonEvery = (2.0f * float(std::numbers::pi)) / float(kSubdivision);

	const float kLatEvery = float(std::numbers::pi) / float(kSubdivision);

	// 緯度方向に分割
	for (int latIndex = 0; latIndex < kSubdivision; ++latIndex) {

		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;

		// 経度方向に分割
		for (int lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {

			float lon = lonIndex * kLonEvery;

			// a
			Vector3 a{
				sphere.center.x +
				sphere.radius * std::cos(lat) * std::cos(lon),

				sphere.center.y +
				sphere.radius * std::sin(lat),

				sphere.center.z +
				sphere.radius * std::cos(lat) * std::sin(lon)
			};

			// b
			Vector3 b{
				sphere.center.x +
				sphere.radius * std::cos(lat + kLatEvery) * std::cos(lon),

				sphere.center.y +
				sphere.radius * std::sin(lat + kLatEvery),

				sphere.center.z +
				sphere.radius * std::cos(lat + kLatEvery) * std::sin(lon)
			};

			// c
			Vector3 c{
				sphere.center.x +
				sphere.radius * std::cos(lat) * std::cos(lon + kLonEvery),

				sphere.center.y +
				sphere.radius * std::sin(lat),

				sphere.center.z +
				sphere.radius * std::cos(lat) * std::sin(lon + kLonEvery)
			};

			// スクリーン座標変換
			Vector3 screenA = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);

			Vector3 screenB = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);

			Vector3 screenC = Transform(Transform(c, viewProjectionMatrix), viewportMatrix);

			// 線を引く
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenB.x), int(screenB.y), color);
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenC.x), int(screenC.y), color);
		}
	}
}