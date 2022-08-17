#include "Vector3.h"
#include <cmath>

Vector3& Vector3::operator*(float s)
{
	Vector3 temp(*this);
	temp.x *= s;
	temp.y *= s;
	temp.z *= s;
	return temp;
}

Vector3& Vector3::operator*(const Matrix4& m) {
	Vector3 temp(*this);

	temp.x = this->x * m.m[0][0] + this->y * m.m[1][0] + this->z * m.m[2][0];
	temp.y = this->x * m.m[0][1] + this->y * m.m[1][1] + this->z * m.m[2][1];
	temp.z = this->x * m.m[0][2] + this->y * m.m[1][2] + this->z * m.m[2][2];

	return temp;
}



const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	temp.x = v1.x * v2.x;
	temp.y = v1.y * v2.y;
	temp.z = v1.z * v2.z;
	return temp;
}

float Vector3::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0) {
		return *this /= length();
	}

	return *this;
}