#pragma once
#include "CannonBall.h"
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

#include <vector>
#include <memory>
#include <iostream>
#include <math.h>

#define ISLAND_BOTTOM 0
#define CANNON_HEIGHT 3
#define STARTING_CANNON_ANGLE 90

#define CANNON_VELOCITY 12

#define ISLAND_CANNON_COOLDOWN_TIME 1

class Island
{
	private:
		float left;
		float right;
		float cannonLeft, cannonRight, cannonAngle;
		float cannonX, cannonY;
		float  cannonCooldownTimer;
		bool coolingDown;
	public:
		float height;
		float collisionRadius, midPoint;

		int maxCannonBalls;
		int health;
		std::vector<std::unique_ptr<CannonBall>> cannonBalls;
		Island(float height, float left, float right, int islandHealth);
		void draw(float resolution);
		void idle(float changeTime);
		void checkInput(unsigned char input);
		virtual ~Island();

};

