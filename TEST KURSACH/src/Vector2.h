#pragma once
class Vector2 {
public:
	unsigned int Y;
	unsigned int X;
	Vector2();
	Vector2(const unsigned int& X, const unsigned int& Y);
	Vector2(const Vector2& other);
	~Vector2();
};