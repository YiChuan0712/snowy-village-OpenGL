#define NDEBUG
#include"collision.h"
#include<math.h>
const float PI = 3.1415926536f;

float min(float x, float y)
{
	return x < y ? x : y;
}

float max(float x, float y)
{
	return x > y ? x : y;
}

struct dot {
	float x;
	float y;
	dot(float _x, float _y) :x(_x), y(_y) { }
};

void move(float* eye, float* center, direction dir)
{
	const float d = 1.0f;
	float x1, x2, y1, y2, x, y;
	x1 = eye[0], y1 = eye[2], x2 = center[0], y2 = center[2];
	float len = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	if (x2 - x1<0.1f&&x2 - x1>-0.1f)x2 = x1;
	if (y2 - y1 < 0.1f&&y2 - y1>-0.1f)y2 = y1;
	switch (dir) {
	case front: {
		eye[0] = d * (x2 - x1) / len + x1;
		eye[2] = d * (y2 - y1) / len + y1;
		center[0] = eye[0] + x2 - x1;
		center[2] = eye[2] + y2 - y1;
		break;
	}
	case back: {
		eye[0] = d * (x1 - x2) / len + x1;
		eye[2] = d * (y1 - y2) / len + y1;
		center[0] = eye[0] + x2 - x1;
		center[2] = eye[2] + y2 - y1;
		break;
	}
	case left: {
		eye[0] = d * (y2 - y1) / len + x1;
		eye[2] = d * (x1 - x2) / len + y1;
		center[0] = eye[0] + x2 - x1;
		center[2] = eye[2] + y2 - y1;
		break;
	}
	case right: {
		eye[0] = d * (y1 - y2) / len + x1;
		eye[2] = d * (x2 - x1) / len + y1;
		center[0] = eye[0] + x2 - x1;
		center[2] = eye[2] + y2 - y1;

		break;
	}
	default:break;
	}
	return;
}

void rotate(float* eye, float* center, direction dir)
{
	const float alpha = 1.0f / (10 * PI);
	float x1, x2, x3, y1, y2, y3;
	float worldcenter[] = { 0, 0, 0 };
	x1 = worldcenter[0], y1 = worldcenter[2], x2 = center[0], y2 = center[2], x3 = eye[0], y3 = eye[2];
	switch (dir) {
	case left: {
		center[0] = x1 + (x2 - x1)*cos(alpha) + (y2 - y1)*sin(alpha);
		center[2] = y1 + (y2 - y1)*cos(alpha) - (x2 - x1)*sin(alpha);

		eye[0] = x1 + (x3 - x1)*cos(alpha) + (y3 - y1)*sin(alpha);
		eye[2] = y1 + (y3 - y1)*cos(alpha) - (x3 - x1)*sin(alpha);
		break;
	}
	case right: {
		center[0] = x1 + (x2 - x1)*cos(alpha) - (y2 - y1)*sin(alpha);
		center[2] = y1 + (y2 - y1)*cos(alpha) + (x2 - x1)*sin(alpha);

		eye[0] = x1 + (x3 - x1)*cos(alpha) - (y3 - y1)*sin(alpha);
		eye[2] = y1 + (y3 - y1)*cos(alpha) + (x3 - x1)*sin(alpha);
		break;
	}
	default:break;
	}
}


