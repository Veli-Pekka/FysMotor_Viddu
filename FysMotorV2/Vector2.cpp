#include "Vector2.h"

Vector2::Vector2()
{

}

Vector2::Vector2(float X, float Y)
{
	x = X;
	y = Y;
}

Vector2::~Vector2()
{

}

Vector2 & Vector2::operator+=(const Vector2& v1)
{
	x = x + v1.x;
	y = y + v1.y;

	return *this;
}

Vector2 & Vector2::operator-=(const Vector2& v1)
{
	x = x - v1.x;
	y = y - v1.y;

	return *this;
}

Vector2 Vector2::operator+(const Vector2 & v1)
{
	return Vector2((x+v1.x),(y+v1.y));
}

Vector2 Vector2::operator-(const Vector2 & v1)
{
	return Vector2((x-v1.x),(y-v1.y));
}

bool Vector2::operator<(const Vector2 & v1)
{
	if (x < v1.x && y < v1.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector2::operator>(const Vector2 & v1)
{
	if (x > v1.x && y > v1.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector2::operator>(const double & vPituus)
{
	double vec_Pituus = 0;

	vec_Pituus = sqrt((x*x) + (y*y));
	if (vec_Pituus < 0.f)
	{
		return false;
	}

	if (vec_Pituus > (vPituus * vPituus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vector2 Vector2::operator*(const Vector2 & v1)
{
	return Vector2();
}
