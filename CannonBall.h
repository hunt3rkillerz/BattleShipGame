#pragma once
#if _WIN32
#include <Windows.h>
#endif
#if __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <math.h>
#define START_TIME 0
#define GRAVITY -10
#define AIM_TES 4096
#define DEG2RAD 3.14159 / 180
class CannonBall
{
	public:
		float curX;
		float curY;
		float angle;
		float radius;
		float velocityY, velocityX;


		CannonBall(float startX, float startY, float velocity, float angle, float radius);
		virtual ~CannonBall();
		void draw(float resolution);
		void idle(float changeTime);
};

