#include "MT3.h"
#include "Novice.h"
#include <cmath>
#include <numbers>
#include <algorithm>

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

Vector3 Perpendicular(const Vector3& vector) {

	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}

	return { 0.0f, -vector.z, vector.y };
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

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb1, const Sphere& sphere) {
	Vector3 closestpoint{
		std::clamp(sphere.center.x,aabb1.min.x,aabb1.max.x),
		std::clamp(sphere.center.y,aabb1.min.y,aabb1.max.y),
		std::clamp(sphere.center.z,aabb1.min.z,aabb1.max.z)
	};
	float distance = Length(Vector3{ (closestpoint.x - sphere.center.x), (closestpoint.y - sphere.center.y), (closestpoint.z - sphere.center.z) });

	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Segment& segment) {
	float txMin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float txMax = (aabb.max.x - segment.origin.x) / segment.diff.x;

	float tyMin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tyMax = (aabb.max.y - segment.origin.y) / segment.diff.y;

	float tzMin = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tzMax = (aabb.max.z - segment.origin.z) / segment.diff.z;

	float tNearX = min(txMin, txMax);
	float tFarX = max(txMin, txMax);

	float tNearY = min(tyMin, tyMax);
	float tFarY = max(tyMin, tyMax);

	float tNearZ = min(tzMin, tzMax);
	float tFarZ = max(tzMin, tzMax);

	float tMin = max(max(tNearX, tNearY), tNearZ);
	float tMax = min(min(tFarX, tFarY), tFarZ);

	if (tMin <= tMax && tMin <= 1.0f && tMax >= 0.0f) {
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Ray& ray) {

	float txMin = (aabb.min.x - ray.origin.x) / ray.diff.x;
	float txMax = (aabb.max.x - ray.origin.x) / ray.diff.x;

	float tyMin = (aabb.min.y - ray.origin.y) / ray.diff.y;
	float tyMax = (aabb.max.y - ray.origin.y) / ray.diff.y;

	float tzMin = (aabb.min.z - ray.origin.z) / ray.diff.z;
	float tzMax = (aabb.max.z - ray.origin.z) / ray.diff.z;

	float tNearX = min(txMin, txMax);
	float tFarX = max(txMin, txMax);

	float tNearY = min(tyMin, tyMax);
	float tFarY = max(tyMin, tyMax);

	float tNearZ = min(tzMin, tzMax);
	float tFarZ = max(tzMin, tzMax);

	float tMin = max(max(tNearX, tNearY), tNearZ);
	float tMax = min(min(tFarX, tFarY), tFarZ);

	if (tMin <= tMax && tMax >= 0.0f) {
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Line& line) {

	float txMin = (aabb.min.x - line.origin.x) / line.diff.x;
	float txMax = (aabb.max.x - line.origin.x) / line.diff.x;

	float tyMin = (aabb.min.y - line.origin.y) / line.diff.y;
	float tyMax = (aabb.max.y - line.origin.y) / line.diff.y;

	float tzMin = (aabb.min.z - line.origin.z) / line.diff.z;
	float tzMax = (aabb.max.z - line.origin.z) / line.diff.z;

	float tNearX = min(txMin, txMax);
	float tFarX = max(txMin, txMax);

	float tNearY = min(tyMin, tyMax);
	float tFarY = max(tyMin, tyMax);

	float tNearZ = min(tzMin, tzMax);
	float tFarZ = max(tzMin, tzMax);

	float tMin = max(max(tNearX, tNearY), tNearZ);
	float tMax = min(min(tFarX, tFarY), tFarZ);

	if (tMin <= tMax) {
		return true;
	}

	return false;
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {
	Vector3 vertices[8];

	vertices[0] = { aabb.min.x, aabb.min.y, aabb.min.z };
	vertices[1] = { aabb.max.x, aabb.min.y, aabb.min.z };
	vertices[2] = { aabb.min.x, aabb.max.y, aabb.min.z };
	vertices[3] = { aabb.max.x, aabb.max.y, aabb.min.z };

	vertices[4] = { aabb.min.x, aabb.min.y, aabb.max.z };
	vertices[5] = { aabb.max.x, aabb.min.y, aabb.max.z };
	vertices[6] = { aabb.min.x, aabb.max.y, aabb.max.z };
	vertices[7] = { aabb.max.x, aabb.max.y, aabb.max.z };

	Vector3 screenVertices[8];

	for (int i = 0; i < 8; ++i) {
		Vector3 ndcVertex = Transform(vertices[i], viewProjectionMatrix);
		screenVertices[i] = Transform(ndcVertex, viewportMatrix);
	}

	// 手前側
	Novice::DrawLine((int)screenVertices[0].x, (int)screenVertices[0].y, (int)screenVertices[1].x, (int)screenVertices[1].y, color);
	Novice::DrawLine((int)screenVertices[1].x, (int)screenVertices[1].y, (int)screenVertices[3].x, (int)screenVertices[3].y, color);
	Novice::DrawLine((int)screenVertices[3].x, (int)screenVertices[3].y, (int)screenVertices[2].x, (int)screenVertices[2].y, color);
	Novice::DrawLine((int)screenVertices[2].x, (int)screenVertices[2].y, (int)screenVertices[0].x, (int)screenVertices[0].y, color);

	// 奥側
	Novice::DrawLine((int)screenVertices[4].x, (int)screenVertices[4].y, (int)screenVertices[5].x, (int)screenVertices[5].y, color);
	Novice::DrawLine((int)screenVertices[5].x, (int)screenVertices[5].y, (int)screenVertices[7].x, (int)screenVertices[7].y, color);
	Novice::DrawLine((int)screenVertices[7].x, (int)screenVertices[7].y, (int)screenVertices[6].x, (int)screenVertices[6].y, color);
	Novice::DrawLine((int)screenVertices[6].x, (int)screenVertices[6].y, (int)screenVertices[4].x, (int)screenVertices[4].y, color);

	// 手前と奥をつなぐ線
	Novice::DrawLine((int)screenVertices[0].x, (int)screenVertices[0].y, (int)screenVertices[4].x, (int)screenVertices[4].y, color);
	Novice::DrawLine((int)screenVertices[1].x, (int)screenVertices[1].y, (int)screenVertices[5].x, (int)screenVertices[5].y, color);
	Novice::DrawLine((int)screenVertices[2].x, (int)screenVertices[2].y, (int)screenVertices[6].x, (int)screenVertices[6].y, color);
	Novice::DrawLine((int)screenVertices[3].x, (int)screenVertices[3].y, (int)screenVertices[7].x, (int)screenVertices[7].y, color);
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

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	Vector3 center = Multiply(plane.distance, plane.normal);

	Vector3 perpendiculars[4];

	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	Vector3 points[4];

	for (int index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);

		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine(
		static_cast<int>(points[0].x),
		static_cast<int>(points[0].y),
		static_cast<int>(points[2].x),
		static_cast<int>(points[2].y),
		color);

	Novice::DrawLine(
		static_cast<int>(points[2].x),
		static_cast<int>(points[2].y),
		static_cast<int>(points[1].x),
		static_cast<int>(points[1].y),
		color);

	Novice::DrawLine(
		static_cast<int>(points[1].x),
		static_cast<int>(points[1].y),
		static_cast<int>(points[3].x),
		static_cast<int>(points[3].y),
		color);

	Novice::DrawLine(
		static_cast<int>(points[3].x),
		static_cast<int>(points[3].y),
		static_cast<int>(points[0].x),
		static_cast<int>(points[0].y),
		color);
}

Matrix4x4 MakeOBBWorldMatrix(const OBB& obb) {

	Matrix4x4 result = MakeIdentity4x4();

	result.m[0][0] = obb.orientations[0].x;
	result.m[0][1] = obb.orientations[0].y;
	result.m[0][2] = obb.orientations[0].z;

	result.m[1][0] = obb.orientations[1].x;
	result.m[1][1] = obb.orientations[1].y;
	result.m[1][2] = obb.orientations[1].z;

	result.m[2][0] = obb.orientations[2].x;
	result.m[2][1] = obb.orientations[2].y;
	result.m[2][2] = obb.orientations[2].z;

	result.m[3][0] = obb.center.x;
	result.m[3][1] = obb.center.y;
	result.m[3][2] = obb.center.z;

	return result;
}

bool IsCollision(const OBB& obb, const Sphere& sphere) {

	Matrix4x4 inverseMatrix = Inverse(MakeOBBWorldMatrix(obb));

	Sphere localSphere{};
	localSphere.center = Transform(sphere.center, inverseMatrix);
	localSphere.radius = sphere.radius;

	AABB localAABB{};
	localAABB.min = { -obb.size.x, -obb.size.y, -obb.size.z };
	localAABB.max = { obb.size.x, obb.size.y, obb.size.z };

	return IsCollision(localAABB, localSphere);
}

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	Vector3 axisX = Multiply(obb.size.x, obb.orientations[0]);
	Vector3 axisY = Multiply(obb.size.y, obb.orientations[1]);
	Vector3 axisZ = Multiply(obb.size.z, obb.orientations[2]);

	Vector3 vertices[8];

	vertices[0] = Add(Add(Add(obb.center, axisX), axisY), axisZ);
	vertices[1] = Add(Add(Subtract(obb.center, axisX), axisY), axisZ);
	vertices[2] = Add(Subtract(Add(obb.center, axisX), axisY), axisZ);
	vertices[3] = Add(Subtract(Subtract(obb.center, axisX), axisY), axisZ);

	vertices[4] = Subtract(Add(Add(obb.center, axisX), axisY), axisZ);
	vertices[5] = Subtract(Add(Subtract(obb.center, axisX), axisY), axisZ);
	vertices[6] = Subtract(Subtract(Add(obb.center, axisX), axisY), axisZ);
	vertices[7] = Subtract(Subtract(Subtract(obb.center, axisX), axisY), axisZ);

	Vector3 screen[8];

	for (int i = 0; i < 8; i++) {
		Vector3 ndc = Transform(vertices[i], viewProjectionMatrix);
		screen[i] = Transform(ndc, viewportMatrix);
	}

	Novice::DrawLine((int)screen[0].x, (int)screen[0].y, (int)screen[1].x, (int)screen[1].y, color);
	Novice::DrawLine((int)screen[1].x, (int)screen[1].y, (int)screen[3].x, (int)screen[3].y, color);
	Novice::DrawLine((int)screen[3].x, (int)screen[3].y, (int)screen[2].x, (int)screen[2].y, color);
	Novice::DrawLine((int)screen[2].x, (int)screen[2].y, (int)screen[0].x, (int)screen[0].y, color);

	Novice::DrawLine((int)screen[4].x, (int)screen[4].y, (int)screen[5].x, (int)screen[5].y, color);
	Novice::DrawLine((int)screen[5].x, (int)screen[5].y, (int)screen[7].x, (int)screen[7].y, color);
	Novice::DrawLine((int)screen[7].x, (int)screen[7].y, (int)screen[6].x, (int)screen[6].y, color);
	Novice::DrawLine((int)screen[6].x, (int)screen[6].y, (int)screen[4].x, (int)screen[4].y, color);

	Novice::DrawLine((int)screen[0].x, (int)screen[0].y, (int)screen[4].x, (int)screen[4].y, color);
	Novice::DrawLine((int)screen[1].x, (int)screen[1].y, (int)screen[5].x, (int)screen[5].y, color);
	Novice::DrawLine((int)screen[2].x, (int)screen[2].y, (int)screen[6].x, (int)screen[6].y, color);
	Novice::DrawLine((int)screen[3].x, (int)screen[3].y, (int)screen[7].x, (int)screen[7].y, color);
}

bool IsCollision(const Segment& segment, const OBB& obb) {

	Matrix4x4 inverseMatrix = Inverse(MakeOBBWorldMatrix(obb));

	Segment localSegment{};
	localSegment.origin = Transform(segment.origin, inverseMatrix);

	Vector3 end = Add(segment.origin, segment.diff);
	Vector3 localEnd = Transform(end, inverseMatrix);

	localSegment.diff = Subtract(localEnd, localSegment.origin);

	AABB localAABB{};
	localAABB.min = { -obb.size.x, -obb.size.y, -obb.size.z };
	localAABB.max = { obb.size.x, obb.size.y, obb.size.z };

	return IsCollision(localAABB, localSegment);
}

bool IsCollision(const Ray& ray, const OBB& obb) {

	Matrix4x4 inverseMatrix = Inverse(MakeOBBWorldMatrix(obb));

	Ray localRay{};
	localRay.origin = Transform(ray.origin, inverseMatrix);

	Vector3 end = Add(ray.origin, ray.diff);
	Vector3 localEnd = Transform(end, inverseMatrix);

	localRay.diff = Subtract(localEnd, localRay.origin);

	AABB localAABB{};
	localAABB.min = { -obb.size.x, -obb.size.y, -obb.size.z };
	localAABB.max = { obb.size.x, obb.size.y, obb.size.z };

	return IsCollision(localAABB, localRay);
}

bool IsCollision(const Line& line, const OBB& obb) {

	Matrix4x4 inverseMatrix = Inverse(MakeOBBWorldMatrix(obb));

	Line localLine{};
	localLine.origin = Transform(line.origin, inverseMatrix);

	Vector3 end = Add(line.origin, line.diff);
	Vector3 localEnd = Transform(end, inverseMatrix);

	localLine.diff = Subtract(localEnd, localLine.origin);

	AABB localAABB{};
	localAABB.min = { -obb.size.x, -obb.size.y, -obb.size.z };
	localAABB.max = { obb.size.x, obb.size.y, obb.size.z };

	return IsCollision(localAABB, localLine);
}

bool IsCollision(const OBB& obb1, const OBB& obb2) {

	Vector3 axes[15];

	axes[0] = obb1.orientations[0];
	axes[1] = obb1.orientations[1];
	axes[2] = obb1.orientations[2];

	axes[3] = obb2.orientations[0];
	axes[4] = obb2.orientations[1];
	axes[5] = obb2.orientations[2];

	int index = 6;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			axes[index++] = Cross(obb1.orientations[i], obb2.orientations[j]);
		}
	}

	for (int i = 0; i < 15; i++) {

		if (Length(axes[i]) <= 0.0001f) {
			continue;
		}

		Vector3 axis = Normalize(axes[i]);

		float distance =
			std::abs(
				Dot(
					Subtract(obb2.center, obb1.center),
					axis));

		float radius1 = 0.0f;

		radius1 += std::abs(Dot(Multiply(obb1.size.x, obb1.orientations[0]), axis));
		radius1 += std::abs(Dot(Multiply(obb1.size.y, obb1.orientations[1]), axis));
		radius1 += std::abs(Dot(Multiply(obb1.size.z, obb1.orientations[2]), axis));

		float radius2 = 0.0f;

		radius2 += std::abs(Dot(Multiply(obb2.size.x, obb2.orientations[0]), axis));
		radius2 += std::abs(Dot(Multiply(obb2.size.y, obb2.orientations[1]), axis));
		radius2 += std::abs(Dot(Multiply(obb2.size.z, obb2.orientations[2]), axis));

		if (distance > radius1 + radius2) {
			return false;
		}
	}

	return true;
}