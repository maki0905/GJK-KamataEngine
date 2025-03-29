#pragma once

#include <cmath>
#include <vector>

struct Vector2 {
	float x, y;

	Vector2 operator-() const { return { -x, -y }; }
	Vector2 operator+(const Vector2& v) const { return { x + v.x, y + v.y }; }
	Vector2 operator-(const Vector2& v) const { return { x - v.x, y - v.y }; }
	Vector2 operator*(float s) const { return { x * s, y * s }; }
	bool operator==(const Vector2& other) const { return (x == other.x) && (y == other.y); }

	float dot(const Vector2& v) const { return x * v.x + y * v.y; }
	float lengthSq() const { return x * x + y * y; }
	float length() const { return std::sqrt(lengthSq()); }
	Vector2 normalize() const {
		float len = length();
		if (len == 0) {
			return { 0.0f, 0.0f };
		}
		else {
			return { x / len, y / len };
		}
	}
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

float Length(const Vector2& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

Vector2 Perpendicular(const Vector2& v) {
	return Vector2(-v.y, v.x);
}

Vector2 ClosestPointOnLineSegment(const Vector2& P1, const Vector2& P2) {
	Vector2 P10 = P1;
	Vector2 P1P2 = P2 - P1;

	float t = -P1P2.dot(P10) / P1P2.dot(P1P2);

	if (t < 0) {
		return P1;
	}
	else if (t > 1) {
		return P2;
	}
	else {
		return P1 + P1P2 * t;
	}
}

struct Face{
	Vector2 a, b, c;
	Vector2 normal;
	float distance;
};

struct Edge {
	Vector2 a, b;
	float distance;
	Vector2 normal;
};
