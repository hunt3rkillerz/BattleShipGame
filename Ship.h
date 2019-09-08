#pragma once
#include "CannonBall.h"
#include "Shield.h"
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
#include<vector>
#include <memory>
#include<math.h>
#include <iostream>
/*
	Ship has 3 components
	Hull
	Roof 
	Cannon
*/
//HULL Constants
#define HULL_BOTTOM -0.2
#define HULL_TOP 0.7
#define HULL_TOP_LEFT_X -1.6
#define HULL_TOP_RIGHT_X 1.6
#define HULL_BOTTOM_LEFT_X -0.8
#define HULL_BOTTOM_RIGHT_X 0.8

//Roof Constants
#define ROOF_TOP 1.66
#define ROOF_BOTTOM 0.7

#define ROOF_RIGHT_X 0.96
#define ROOF_LEFT_X 0.0

//Cannon
#define CANNON_TOP 0.128
#define CANNON_BOTTOM -0.128
#define CANNON_LEFT_X 0
#define CANNON_RIGHT_X -1.28

#define SHIP_STARTING_CANNON_ANGLE 45

#define LEFT_ORIENTATION -1;
#define RIGHT_ORIENTATION 1;

#define SHIP_CANNON_VELOCITY 12
#define SHIP_CANNON_BALL_RADIUS 0.1

#define RADIUS 0.8

#define MAX_CANNON_BALLS 3

#define SHIP_CANNON_COOLDOWN_TIME 1
#define SHIP_SHIELD_COOLDOWN 6
class Ship
{
	private:
		float cannonAngle;
		float realCannonAngle;

	public:
		float scale;
		float orientation;
		float curX, curY;
		float cannonX, cannonY;
		float radius;
		float curBoatRotation;
		float boundLeft, boundRight;
		float shieldTimer;
		float cannonCooldownTimer;

		int maxCannonBalls;
		int health;
		int red, green, blue, opacity;

		//Control Vector
		std::vector<char> controls;
		//Cannon Balls
		std::vector<std::unique_ptr<CannonBall>> cannonBalls;
		//Shield
		std::unique_ptr<Shield> shield;

		bool coolingDown;
		bool  shieldCoolingDown;

		//Functions
		Ship(float scale, float orientation, float startX, int health, float boundLeft, float boundRight);
		void setColor(int r, int g, int b, int o);
		float getRealCannonAngle();

		void checkInput(unsigned char input, float curFrameRate);
		void drawShip(float TranslationY, float boatRotation, float resolution);
		void idle(float changeTime);
		void setShipControls(char forward, char backward, char aimUp, char aimDow, char fire, char shield);
		virtual ~Ship();
};

