#pragma once

struct Vector3 {
	float x;
	float y;
	float z;

	// 複合代入演算子
	Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
};

// 単項演算子
inline Vector3 operator-(const Vector3& v) { return { -v.x, -v.y, -v.z }; }
inline Vector3 operator+(const Vector3& v) { return v; }

// 二項演算子
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) { return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z }; }
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) { return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z }; }
inline Vector3 operator*(float s, const Vector3& v) { return { s * v.x, s * v.y, s * v.z }; }
inline Vector3 operator*(const Vector3& v, float s) { return s * v; }
inline Vector3 operator/(const Vector3& v, float s) { return { v.x / s, v.y / s, v.z / s }; }
