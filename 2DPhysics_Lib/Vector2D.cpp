#include "Vector2D.h"

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

Vector2D::Vector2D(Vector2D& _other)
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
