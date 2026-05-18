#include "Sphere.h"
#include <cmath>
#include <numbers>
#include <Novice.h>

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