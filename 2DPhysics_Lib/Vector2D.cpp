#include "Vector2D.h"
#include <cmath>

Vector2D::Vector2D()
{
	X = 0;
	Y = 0;
}

Vector2D::Vector2D(float _x, float _y)
{
	X = _x;
	Y = _y;
}

Vector2D::Vector2D(const Vector2D& _other)
{
	X = _other.X;
	Y = _other.Y;
}

Vector2D Vector2D::operator+(const Vector2D& _lhs)
{
	Vector2D out;
	out.X = X + _lhs.X;
	out.Y = Y + _lhs.Y;
	return out;
}

Vector2D& Vector2D::operator+=(const Vector2D& _rhs)
{
	this->X += _rhs.X;
	this->Y += _rhs.Y;
	return *this;
}

Vector2D Vector2D::operator-(const Vector2D& _lhs)
{
	Vector2D out;
	out.X = X - _lhs.X;
	out.Y = Y - _lhs.Y;
	return out;
}

Vector2D& Vector2D::operator-=(const Vector2D& _rhs)
{
	this->X -= _rhs.X;
	this->Y -= _rhs.Y;
	return *this;
}

Vector2D Vector2D::operator*(const float& _rhs)
{
	return Vector2D(X * _rhs, Y * _rhs);
}

float Vector2D::Magnitude()
{
	float x2 = X * X;
	float y2 = Y * Y;
	return std::sqrtf(x2 + y2);
}

float Vector2D::AbsMagnitude()
{
	float x2 = X * X;
	float y2 = Y * Y;
	return abs(std::sqrtf(x2 + y2));
}
