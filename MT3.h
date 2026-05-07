#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

inline constexpr int kColumnWidth = 60;
inline constexpr int kRowHeight = 20;

//=================================================================================================//
// Vector3

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

// 内積
float Dot(const Vector3& v1, const Vector3& v2);

// 長さ
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);

// 画面表示
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
//=================================================================================================//


//=================================================================================================//
// Matrix4x4

// 加算
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

// 減算
Matrix4x4 Subtruct(const Matrix4x4& m1, const Matrix4x4& m2);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);

// 単位行列の作成
Matrix4x4 MakeIdentity4x4();

// 画面表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);
//=================================================================================================//