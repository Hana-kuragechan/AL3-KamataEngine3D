#include "Vector3.h"

Vector3 operator+(const Vector3& num1, const Vector3& num2) {
	Vector3 result;

	result.x = num1.x + num2.x;
	result.y = num1.y + num2.y;
	result.z = num1.z + num2.z;

	return result;
}