#include "Vector3.h"

Vector3 Vector3::operator+() const
{
	return *this;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}