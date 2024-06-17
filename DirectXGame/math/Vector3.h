#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
	Vector3& operator+=(const Vector3& num) { 
		
		x += num.x;
		y += num.y;
		z += num.z;
		return *this;
	}
};
Vector3 operator+(const Vector3& num1, const Vector3& num2);
