#ifndef VECTOR_H_
#define VECTOR_H_

#define _USE_MATH_DEFINES
#include <math.h>

class vec2 {
public:
	vec2(): x(0), y(0) {};
	vec2(float x, float y): x(x), y(y) {};
	//vec2(vec2 in): x(in.x), y(in.y) {};

	float x, y;

	void rotate(float ang) {
		double rad = ang * M_PI / 180;

		double cs = cos(rad);
		double sn = sin(rad);

		float nx = (float)(x * cs - y * sn);
		float ny = (float)(x * sn + y * cs);

		x = nx;
		y = ny;
	}

	float length() {
		return (float)sqrt(pow(x, 2) + pow(y, 2));
	}

	friend const vec2 operator+(vec2 a, vec2 b) {
		vec2 ret = {0, 0};
		ret.x = a.x + b.x;
		ret.y = a.y + b.y;
		return ret;
	}

	friend const vec2 operator-(vec2 a, vec2 b) {
		vec2 ret = {0, 0};
		ret.x = a.x - b.x;
		ret.y = a.y - b.y;
		return ret;
	}

	friend const vec2 operator*(vec2 a, vec2 b) {
		vec2 ret = {0, 0};
		ret.x = a.x * b.x;
		ret.y = a.y * b.y;
		return ret;
	}

	friend const vec2 operator*(vec2 a, float b) {
		vec2 ret = {0, 0};
		ret.x = a.x * b;
		ret.y = a.y * b;
		return ret;
	}

	friend const vec2 operator/(vec2 a, float b) {
		vec2 ret = {0, 0};
		ret.x = a.x / b;
		ret.y = a.y / b;
		return ret;
	}

	friend const vec2 operator/(vec2 a, vec2 b) {
		vec2 ret = {0, 0};
		ret.x = a.x / b.x;
		ret.y = a.y / b.y;
		return ret;
	}
};





#endif /* VECTOR_H_ */
