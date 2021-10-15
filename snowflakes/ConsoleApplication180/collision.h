#pragma once
typedef enum { left, right, front, back }direction;
void move(float* eye, float* center, direction dir);
void rotate(float* eye, float* center, direction dir);