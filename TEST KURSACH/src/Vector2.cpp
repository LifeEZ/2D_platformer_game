#include "Vector2.h"

Vector2::Vector2()
	: X(0), Y(0) {
}

Vector2::Vector2(const unsigned int& _X, const unsigned int& _Y)
	: X(_X), Y(_Y) {
}

Vector2::Vector2(const Vector2& other)
	: X(other.X), Y(other.Y) {
}

Vector2::~Vector2() {
}