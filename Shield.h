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
#define START_RADIUS 1
#define DEG2RAD 3.14159 / 180
#define HEIGHT_COVER 2
class Shield
{
	private:
		float curX, curY;
		float radius;
	public:
		Shield(float startX, float startY);
		~Shield();
		void idle(float changeTime);
		void draw(float resolution);
		const float& getRadius();
		const float& getX();
		const float& getY();
};

