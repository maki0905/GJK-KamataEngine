#pragma once

#include <cmath>

struct Vector2
{
	float x;
	float y;

};

Vector2 Add(const Vector2& v1, const Vector2& v2) {
	Vector2 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	return result;
}

Vector2 Subtract(const Vector2& v1, const Vector2& v2) {
	Vector2 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	return result;
}

Vector2 Normalize(const Vector2& v)
{
	Vector2 result;
	float len = std::sqrt(v.x * v.x + v.y * v.y);
	if (len == 0) {
		result = { 0.0f, 0.0f };
	}
	else {
		result = { v.x / len, v.y / len };
	}
	return result;
}

Vector2 Multiply(float k, const Vector2& v)
{
	Vector2 result;
	result.x = k * v.x;
	result.y = k * v.y;
	return result;
}

float Dot(const Vector2& v1, const Vector2& v2)
{
	float result;
	result = v1.x * v2.x + v1.y * v2.y;
	return result;
}

float Cross(const Vector2& v1, const Vector2& v2)
{
	float result = v1.x * v2.y - v1.y * v2.x;
	return result;
}
