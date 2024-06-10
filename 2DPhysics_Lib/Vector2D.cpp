#include "Vector2D.h"

#include "Maths.h"

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

Vector2D operator+(Vector2D _lhs, Vector2D _rhs)
{
	return Vector2D(_lhs.X + _rhs.X, _lhs.Y + _rhs.Y);
}

Vector2D& Vector2D::operator+=(Vector2D _rhs)
{
	this->X += _rhs.X;
	this->Y += _rhs.Y;
	return *this;
}

Vector2D Vector2D::operator-(Vector2D _lhs)
{
	Vector2D out;
	out.X = X - _lhs.X;
	out.Y = Y - _lhs.Y;
	return out;
}

Vector2D& Vector2D::operator-=(Vector2D _rhs)
{
	this->X -= _rhs.X;
	this->Y -= _rhs.Y;
	return *this;
}

Vector2D Vector2D::operator-()
{
	return Vector2D(-X, -Y);
}

Vector2D Vector2D::operator*(float _rhs)
{
	return Vector2D(X * _rhs, Y * _rhs);
}

Vector2D operator*(float _lhs, Vector2D _rhs)
{
	return Vector2D(_lhs * _rhs.X, _lhs * _rhs.Y);
}

Vector2D Vector2D::operator/(float _rhs)
{
	return Vector2D(X / _rhs, Y / _rhs);
}

Vector2D& Vector2D::operator/=(float _rhs)
{
	this->X /= _rhs;
	this->Y /= _rhs;
	return *this;
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

Vector2D& Vector2D::Normalize()
{
	float magnitude = Magnitude();
	magnitude = (magnitude > 0.f ? magnitude : 1.f);
	X /= magnitude;
	Y /= magnitude;
	return *this;
}

Vector2D Vector2D::Normalized(Vector2D& _toNormalize)
{
	float magnitude = _toNormalize.Magnitude();
	magnitude = (magnitude > 0.f ? magnitude : 1.f);
	return Vector2D(_toNormalize.X / magnitude, _toNormalize.Y / magnitude);
}

float Vector2D::Dot(Vector2D _vec1, Vector2D _vec2)
{
	return (_vec1.X * _vec2.X) + (_vec1.Y * _vec2.Y);
}

float Vector2D::Cross(const Vector2D& _vec1, const Vector2D& _vec2)
{
	return (_vec1.X * _vec2.Y) - (_vec1.Y * _vec2.X);
}

Vector2D Vector2D::Cross(const float& _s, const Vector2D& _vec)
{
	return Vector2D(-_s * _vec.Y, _s * _vec.X);
}

Vector2D Vector2D::Cross(const Vector2D& _vec, const float& _s)
{
	return Vector2D(_s * _vec.Y, -_s * _vec.X);
}

Vector2D Vector2D::AngleToUnitVector(const float& _angle)
{
	const float radians = Physics2D::Deg2Rad(_angle);
	return Vector2D(std::sinf(radians), std::cosf(radians)).Normalize();
}

float Vector2D::Distance(const Vector2D& _vec1, const Vector2D& _vec2)
{
	float xDiff = std::powf(_vec2.X - _vec1.X, 2.f);
	float yDiff = std::powf(_vec2.Y - _vec1.Y, 2.f);

	return std::sqrtf(xDiff + yDiff);
}

Vector2D Vector2D::PerpendicularVector(const Vector2D& _vec)
{
	return Vector2D(_vec.Y, -_vec.X);
}
