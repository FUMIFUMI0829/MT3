#include "MT3.h"
#include "Novice.h"
#include <cmath>

//=================================================================================================//
// Vector3

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 result{};

	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;

	return result;
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	float result = 0.0f;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return result;
}

// 長さ
float Length(const Vector3& v) {
	float result = 0.0f;

	result = std::sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	return result;
}

// 正規化
Vector3 Normalize(const Vector3& v) {
	Vector3 result{};
	float length = Length(v);

	if (length == 0.0f) {
		return result;
	}

	result.x = v.x / length;
	result.y = v.y / length;
	result.z = v.z / length;

	return result;
}

// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];

	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];

	if (w != 0.0f) {
		result.x /= w;
		result.y /= w;
		result.z /= w;
	}

	return result;
}

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float dot = Dot(v1, v2);
	float length = Dot(v2, v2);

	Vector3 result = Multiply(dot / length, v2);

	return result;
}

// 最近接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 originToPoint = Subtract(point, segment.origin);

	Vector3 project = Project(originToPoint, segment.diff);

	Vector3 closestPoint = Add(segment.origin, project);

	return closestPoint;
}

// 画面表示
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}
//=================================================================================================//


//=================================================================================================//
// Matrix4x4

// 加算
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultMatrix{};

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			resultMatrix.m[row][col] = m1.m[row][col] + m2.m[row][col];
		}
	}

	return resultMatrix;
}

// 減算
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultMatrix{};

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			resultMatrix.m[row][col] = m1.m[row][col] - m2.m[row][col];
		}
	}

	return resultMatrix;
}

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultMatrix{};

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			resultMatrix.m[row][col] = 0.f;

			for (int k = 0; k < 4; ++k) {
				resultMatrix.m[row][col] += m1.m[row][k] * m2.m[k][col];
			}
		}
	}

	return resultMatrix;
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	float a =
		m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]
		- m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]
		- m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]
		+ m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]
		+ m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]
		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]
		+ m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]
		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]
		- m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]
		+ m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]
		+ m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]
		- m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]
		- m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]
		+ m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]
		+ m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]
		- m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]
		- m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0];

	Matrix4x4 resultMatrix{};

	resultMatrix.m[0][0] = (
		+m.m[1][1] * m.m[2][2] * m.m[3][3]
		- m.m[1][1] * m.m[2][3] * m.m[3][2]
		- m.m[1][2] * m.m[2][1] * m.m[3][3]
		+ m.m[1][2] * m.m[2][3] * m.m[3][1]
		+ m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[1][3] * m.m[2][2] * m.m[3][1]) / a;

	resultMatrix.m[0][1] = (
		-m.m[0][1] * m.m[2][2] * m.m[3][3]
		+ m.m[0][1] * m.m[2][3] * m.m[3][2]
		+ m.m[0][2] * m.m[2][1] * m.m[3][3]
		- m.m[0][2] * m.m[2][3] * m.m[3][1]
		- m.m[0][3] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[2][2] * m.m[3][1]) / a;

	resultMatrix.m[0][2] = (
		+m.m[0][1] * m.m[1][2] * m.m[3][3]
		- m.m[0][1] * m.m[1][3] * m.m[3][2]
		- m.m[0][2] * m.m[1][1] * m.m[3][3]
		+ m.m[0][2] * m.m[1][3] * m.m[3][1]
		+ m.m[0][3] * m.m[1][1] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[3][1]) / a;

	resultMatrix.m[0][3] = (
		-m.m[0][1] * m.m[1][2] * m.m[2][3]
		+ m.m[0][1] * m.m[1][3] * m.m[2][2]
		+ m.m[0][2] * m.m[1][1] * m.m[2][3]
		- m.m[0][2] * m.m[1][3] * m.m[2][1]
		- m.m[0][3] * m.m[1][1] * m.m[2][2]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1]) / a;

	resultMatrix.m[1][0] = (
		-m.m[1][0] * m.m[2][2] * m.m[3][3]
		+ m.m[1][0] * m.m[2][3] * m.m[3][2]
		+ m.m[1][2] * m.m[2][0] * m.m[3][3]
		- m.m[1][2] * m.m[2][3] * m.m[3][0]
		- m.m[1][3] * m.m[2][0] * m.m[3][2]
		+ m.m[1][3] * m.m[2][2] * m.m[3][0]) / a;

	resultMatrix.m[1][1] = (
		+m.m[0][0] * m.m[2][2] * m.m[3][3]
		- m.m[0][0] * m.m[2][3] * m.m[3][2]
		- m.m[0][2] * m.m[2][0] * m.m[3][3]
		+ m.m[0][2] * m.m[2][3] * m.m[3][0]
		+ m.m[0][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[2][2] * m.m[3][0]) / a;

	resultMatrix.m[1][2] = (
		-m.m[0][0] * m.m[1][2] * m.m[3][3]
		+ m.m[0][0] * m.m[1][3] * m.m[3][2]
		+ m.m[0][2] * m.m[1][0] * m.m[3][3]
		- m.m[0][2] * m.m[1][3] * m.m[3][0]
		- m.m[0][3] * m.m[1][0] * m.m[3][2]
		+ m.m[0][3] * m.m[1][2] * m.m[3][0]) / a;

	resultMatrix.m[1][3] = (
		+m.m[0][0] * m.m[1][2] * m.m[2][3]
		- m.m[0][0] * m.m[1][3] * m.m[2][2]
		- m.m[0][2] * m.m[1][0] * m.m[2][3]
		+ m.m[0][2] * m.m[1][3] * m.m[2][0]
		+ m.m[0][3] * m.m[1][0] * m.m[2][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0]) / a;

	resultMatrix.m[2][0] = (
		+m.m[1][0] * m.m[2][1] * m.m[3][3]
		- m.m[1][0] * m.m[2][3] * m.m[3][1]
		- m.m[1][1] * m.m[2][0] * m.m[3][3]
		+ m.m[1][1] * m.m[2][3] * m.m[3][0]
		+ m.m[1][3] * m.m[2][0] * m.m[3][1]
		- m.m[1][3] * m.m[2][1] * m.m[3][0]) / a;

	resultMatrix.m[2][1] = (
		-m.m[0][0] * m.m[2][1] * m.m[3][3]
		+ m.m[0][0] * m.m[2][3] * m.m[3][1]
		+ m.m[0][1] * m.m[2][0] * m.m[3][3]
		- m.m[0][1] * m.m[2][3] * m.m[3][0]
		- m.m[0][3] * m.m[2][0] * m.m[3][1]
		+ m.m[0][3] * m.m[2][1] * m.m[3][0]) / a;

	resultMatrix.m[2][2] = (
		+m.m[0][0] * m.m[1][1] * m.m[3][3]
		- m.m[0][0] * m.m[1][3] * m.m[3][1]
		- m.m[0][1] * m.m[1][0] * m.m[3][3]
		+ m.m[0][1] * m.m[1][3] * m.m[3][0]
		+ m.m[0][3] * m.m[1][0] * m.m[3][1]
		- m.m[0][3] * m.m[1][1] * m.m[3][0]) / a;

	resultMatrix.m[2][3] = (
		-m.m[0][0] * m.m[1][1] * m.m[2][3]
		+ m.m[0][0] * m.m[1][3] * m.m[2][1]
		+ m.m[0][1] * m.m[1][0] * m.m[2][3]
		- m.m[0][1] * m.m[1][3] * m.m[2][0]
		- m.m[0][3] * m.m[1][0] * m.m[2][1]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0]) / a;

	resultMatrix.m[3][0] = (
		-m.m[1][0] * m.m[2][1] * m.m[3][2]
		+ m.m[1][0] * m.m[2][2] * m.m[3][1]
		+ m.m[1][1] * m.m[2][0] * m.m[3][2]
		- m.m[1][1] * m.m[2][2] * m.m[3][0]
		- m.m[1][2] * m.m[2][0] * m.m[3][1]
		+ m.m[1][2] * m.m[2][1] * m.m[3][0]) / a;

	resultMatrix.m[3][1] = (
		+m.m[0][0] * m.m[2][1] * m.m[3][2]
		- m.m[0][0] * m.m[2][2] * m.m[3][1]
		- m.m[0][1] * m.m[2][0] * m.m[3][2]
		+ m.m[0][1] * m.m[2][2] * m.m[3][0]
		+ m.m[0][2] * m.m[2][0] * m.m[3][1]
		- m.m[0][2] * m.m[2][1] * m.m[3][0]) / a;

	resultMatrix.m[3][2] = (
		-m.m[0][0] * m.m[1][1] * m.m[3][2]
		+ m.m[0][0] * m.m[1][2] * m.m[3][1]
		+ m.m[0][1] * m.m[1][0] * m.m[3][2]
		- m.m[0][1] * m.m[1][2] * m.m[3][0]
		- m.m[0][2] * m.m[1][0] * m.m[3][1]
		+ m.m[0][2] * m.m[1][1] * m.m[3][0]) / a;

	resultMatrix.m[3][3] = (
		+m.m[0][0] * m.m[1][1] * m.m[2][2]
		- m.m[0][0] * m.m[1][2] * m.m[2][1]
		- m.m[0][1] * m.m[1][0] * m.m[2][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0]
		+ m.m[0][2] * m.m[1][0] * m.m[2][1]
		- m.m[0][2] * m.m[1][1] * m.m[2][0]) / a;

	return resultMatrix;
}

// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 resultMatrix{};

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			resultMatrix.m[row][col] = m.m[col][row];
		}
	}

	return resultMatrix;
}

// 単位行列の作成
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 resultMatrix{};

	for (int i = 0; i < 4; ++i) {
		resultMatrix.m[i][i] = 1.f;
	}

	return resultMatrix;
}

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 resultMatrix = MakeIdentity4x4();

	resultMatrix.m[3][0] = translate.x;
	resultMatrix.m[3][1] = translate.y;
	resultMatrix.m[3][2] = translate.z;

	return resultMatrix;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 resultMatrix{};

	resultMatrix.m[0][0] = scale.x;
	resultMatrix.m[1][1] = scale.y;
	resultMatrix.m[2][2] = scale.z;
	resultMatrix.m[3][3] = 1.0f;

	return resultMatrix;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 resultMatrix = MakeIdentity4x4();

	resultMatrix.m[1][1] = std::cos(radian);
	resultMatrix.m[1][2] = std::sin(radian);

	resultMatrix.m[2][1] = -std::sin(radian);
	resultMatrix.m[2][2] = std::cos(radian);

	return resultMatrix;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 resultMatrix = MakeIdentity4x4();

	resultMatrix.m[0][0] = std::cos(radian);
	resultMatrix.m[0][2] = -std::sin(radian);

	resultMatrix.m[2][0] = std::sin(radian);
	resultMatrix.m[2][2] = std::cos(radian);

	return resultMatrix;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 resultMatrix = MakeIdentity4x4();

	resultMatrix.m[0][0] = std::cos(radian);
	resultMatrix.m[0][1] = std::sin(radian);

	resultMatrix.m[1][0] = -std::sin(radian);
	resultMatrix.m[1][1] = std::cos(radian);

	return resultMatrix;
}

// アフィン変換
Matrix4x4  MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	// 拡大縮小行列
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	// 回転行列
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	// 平行移動行列
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	// 回転行列を合成
	Matrix4x4 rotateMatrix = Multiply(Multiply(rotateXMatrix, rotateYMatrix), rotateZMatrix);

	// SRT
	Matrix4x4 srMatrix = Multiply(scaleMatrix, rotateMatrix);
	Matrix4x4 srtMatrix = Multiply(srMatrix, translateMatrix);

	return srtMatrix;
}

// 透視射影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 resultMatrix{};
	float f = 1.f / std::tanf(fovY / 2.f);
	resultMatrix.m[0][0] = f / aspectRatio;
	resultMatrix.m[1][1] = f;
	resultMatrix.m[2][2] = farClip / (farClip - nearClip);
	resultMatrix.m[2][3] = 1.f;
	resultMatrix.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

	return resultMatrix;
}

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 resultMatrix{};
	resultMatrix.m[0][0] = 2.f / (right - left);
	resultMatrix.m[1][1] = 2.f / (top - bottom);
	resultMatrix.m[2][2] = 1.f / (farClip - nearClip);
	resultMatrix.m[3][0] = (left + right) / (left - right);
	resultMatrix.m[3][1] = (top + bottom) / (bottom - top);
	resultMatrix.m[3][2] = nearClip / (nearClip - farClip);
	resultMatrix.m[3][3] = 1.f;

	return resultMatrix;
}

// ビューポート行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 resultMatrix{};
	resultMatrix.m[0][0] = width / 2.f;
	resultMatrix.m[1][1] = -height / 2.f;
	resultMatrix.m[2][2] = maxDepth - minDepth;
	resultMatrix.m[3][0] = left + (width / 2.f);
	resultMatrix.m[3][1] = top + (height / 2.f);
	resultMatrix.m[3][2] = minDepth;
	resultMatrix.m[3][3] = 1.f;

	return resultMatrix;
}

// 画面表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			Novice::ScreenPrintf(x + col * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][col]);
		}
	}
}
//=================================================================================================//

//=================================================================================================//
// Grid表示

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.f;
	const int kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.f) / float(kSubdivision);
	// 奥から手前への線を徐々に引いていく
	for (int xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		// 今引いている線の座標
		float x = -kGridHalfWidth + (xIndex * kGridEvery);

		// ワールド座標
		Vector3 start = { x,0.0f,-kGridHalfWidth };
		Vector3 end = { x,0.0f, kGridHalfWidth };

		// スクリーン座標変換
		Vector3 screenStart = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 screenEnd = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		// 線を引く
		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), 0x000000FF);
	}
	// 左から右への線を徐々に引いていく
	for (int zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// 今引いている線の座標
		float z = -kGridHalfWidth + (zIndex * kGridEvery);

		// ワールド座標
		Vector3 start = { -kGridHalfWidth,0.0f,z };
		Vector3 end = { kGridHalfWidth,0.0f, z };

		// スクリーン座標変換
		Vector3 screenStart = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 screenEnd = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		// 線を引く
		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), 0x000000FF);
	}

}
//=================================================================================================//
bool IsCollision(const Sphere& sphere, const Plane& plane) {

	float distance = Dot(plane.normal, sphere.center) - plane.distance;

	if (std::abs(distance) <= sphere.radius) {
		return true;
	}

	return false;
}

bool IsCollision(const Sphere& sphere1, const Sphere& sphere2) {

	float distance = Length(Subtract(sphere1.center, sphere2.center));

	if (distance <= sphere1.radius + sphere2.radius) {
		return true;
	}

	return false;
}

bool IsCollision(const Segment& segment, const Plane& plane) {

	float start = Dot(segment.origin, plane.normal) - plane.distance;

	Vector3 endPoint = Add(segment.origin, segment.diff);

	float end = Dot(endPoint, plane.normal) - plane.distance;

	if (start * end <= 0.0f) {
		return true;
	}

	return false;
}

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	Vector3 start = segment.origin;
	Vector3 end = Add(segment.origin, segment.diff);

	start = Transform(start, viewProjectionMatrix);
	start = Transform(start, viewportMatrix);

	end = Transform(end, viewProjectionMatrix);
	end = Transform(end, viewportMatrix);

	Novice::DrawLine(
		static_cast<int>(start.x),
		static_cast<int>(start.y),
		static_cast<int>(end.x),
		static_cast<int>(end.y),
		color
	);
}

bool IsCollision(const Triangle& triangle, const Segment& segment) {

	// 三角形の法線を求める
	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v02 = Subtract(triangle.vertices[2], triangle.vertices[0]);

	Vector3 normal = Normalize(Cross(v01, v02));

	// 三角形がある平面を作る
	Plane plane{};
	plane.normal = normal;
	plane.distance = Dot(triangle.vertices[0], normal);

	// 線分と平面が衝突していなければ終了
	if (IsCollision(segment, plane) == false) {
		return false;
	}

	// 線分と平面の交点を求める
	float start = Dot(segment.origin, plane.normal) - plane.distance;
	float end = Dot(Add(segment.origin, segment.diff), plane.normal) - plane.distance;

	float t = start / (start - end);

	Vector3 point = Add(segment.origin, Multiply(t, segment.diff));

	// 三角形の各辺ベクトル
	Vector3 edge01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 edge12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 edge20 = Subtract(triangle.vertices[0], triangle.vertices[2]);

	// 各頂点から交点へのベクトル
	Vector3 v0p = Subtract(point, triangle.vertices[0]);
	Vector3 v1p = Subtract(point, triangle.vertices[1]);
	Vector3 v2p = Subtract(point, triangle.vertices[2]);

	// 外積
	Vector3 cross01 = Cross(edge01, v0p);
	Vector3 cross12 = Cross(edge12, v1p);
	Vector3 cross20 = Cross(edge20, v2p);

	// 全部法線と同じ向きなら三角形の内側
	if (Dot(cross01, normal) >= 0.0f && Dot(cross12, normal) >= 0.0f && Dot(cross20, normal) >= 0.0f) {
		return true;
	}

	return false;
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	Vector3 screenVertices[3]{};

	for (int i = 0; i < 3; ++i) {
		Vector3 ndcVertex = Transform(triangle.vertices[i], viewProjectionMatrix);
		screenVertices[i] = Transform(ndcVertex, viewportMatrix);
	}

	Novice::DrawTriangle(
		static_cast<int>(screenVertices[0].x),
		static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[1].x),
		static_cast<int>(screenVertices[1].y),
		static_cast<int>(screenVertices[2].x),
		static_cast<int>(screenVertices[2].y),
		color,
		kFillModeWireFrame);
}