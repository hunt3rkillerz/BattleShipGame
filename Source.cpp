#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include <string>
#include<math.h>
#include<time.h>
#include <memory>
#include <sstream>
#include <iomanip>
#include <string>
#include "Ship.h"
#include "Island.h"
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

//Game Constants
#define MAX_WIDTH 20.0
#define MAX_HEIGHT 20.0
#define MAX_DEPTH 20.0
#define PI 3.14159265
#define ESCAPE_KEY 27

//Wave Constants
#define WAVE_HEIGHT 10
#define WAVE_VELOCITY 1.5
#define WAVE_AMPLITUDE 2
#define WAVE_FREQUENCY 1
#define SIMPLE_TES 16

//Island Constants
#define ISLAND_HEIGHT WAVE_HEIGHT + WAVE_AMPLITUDE
#define	ISLAND_LEFT 7
#define ISLAND_RIGHT 13
#define ISLAND_HEALTH 100

//Ship Constants
#define SHIP_HEALTH 10
#define BOAT_SCALE 0.5

//Global Variables
//Ideally I would turn most of this file into a game object to clean this up

//Resolution
float resolution;

//FrameRate and frame counter
float curFrameRate, frames;
float waveDisplacement;

//State Information
bool running = true;
bool normals = false;
bool tangents = false;
bool wireFrame = false;
bool OSD = false;
//Ships
std::unique_ptr<Ship> leftShip, rightShip;
//The Island
std::unique_ptr<Island> island;

/*******************************
		Util Methods Start
********************************/
void calcFrameRate(float changeTime)
{
	curFrameRate = frames / changeTime;
	frames = 0;
}

void drawAxes()
{
	//X
	glBegin(GL_LINES);
	glColor3f(255, 0, 0);
	glVertex3f(MAX_WIDTH / 2, MAX_HEIGHT / 2, 0);
	glVertex3f(MAX_WIDTH, MAX_HEIGHT / 2, 0);
	glEnd();
	//Y
	glBegin(GL_LINES);
	glColor3f(0, 255, 0);
	glVertex3f(MAX_WIDTH / 2, MAX_HEIGHT / 2, 0);
	glVertex3f(MAX_WIDTH / 2, MAX_HEIGHT, 0);
	glEnd();
	//Z
	glBegin(GL_LINES);
	glColor3f(0, 0, 255);
	glVertex3f(MAX_WIDTH / 2, MAX_HEIGHT / 2, 0);
	glVertex3f(MAX_WIDTH / 2, MAX_HEIGHT / 2, MAX_DEPTH);
	glEnd();
}

float diffSinWave(float A, float k, float c, float x)
{
	return atan(A * k * cos((k * x) - waveDisplacement)) * 180/PI;
}

float sinWave(float A, float k, float c, float x)
{
	return A * (sin((k*x) - waveDisplacement)) + c;
}

void waveMove(float changeTime)
{
	waveDisplacement += WAVE_VELOCITY * changeTime;
}
/*******************************
		Util Methods End
********************************/

/*******************************
		Draw Methods Start
********************************/
void drawWater(float A, float k, float c, float res)
{
	glBegin(GL_QUAD_STRIP);
	glColor4ub(0, 0, 255, 100); 
	for (float i = 0; i <= res; i++)
	{
		float x = i * (MAX_WIDTH / res);
		float y = sinWave(A, k, c, x);
		glVertex3f(x, y, 2);
		glVertex3f(x, -1, 2);
	}
	glEnd();
}

void drawShip(Ship *ship, float sinWave, float diffSin)
{
	ship->drawShip(sinWave, diffSin, resolution);
}

void drawTangents()
{
	glColor4ub(255, 106, 0, 255);
	float step = MAX_WIDTH / SIMPLE_TES;
	for (float i = 0; i <= SIMPLE_TES; i++)
	{
		float x = i * step - 1;
		float y = sinWave(WAVE_AMPLITUDE, WAVE_FREQUENCY, WAVE_HEIGHT, x);
		float angle = diffSinWave(WAVE_AMPLITUDE, WAVE_FREQUENCY, WAVE_HEIGHT, x);

		glPushMatrix();

		glTranslatef(x, y, 0);
		glScalef(0.25, 0.25, 1);
		glRotatef(angle, 0, 0, 1.0);

		glBegin(GL_LINES);
		glVertex3f(-1, 0, 5.5);
		glVertex3f(1, 0, 5.5);
		glEnd();

		glPopMatrix();
	}
}

void drawNormals()
{
	glColor4ub(0, 255, 255, 255);
	float step = MAX_WIDTH / SIMPLE_TES;
	for (float i = 0; i <= SIMPLE_TES; i++)
	{
		float x = i * step - 1;
		float y = sinWave(WAVE_AMPLITUDE, WAVE_FREQUENCY, WAVE_HEIGHT, x);
		float angle = diffSinWave(WAVE_AMPLITUDE, WAVE_FREQUENCY, WAVE_HEIGHT, x) + 90;

		glPushMatrix();

		glTranslatef(x, y, 0);
		glScalef(0.25, 0.25, 1);
		glRotatef(angle, 0, 0, 1.0);

		glBegin(GL_LINES);
		glVertex3f(-1, 0, 5.5);
		glVertex3f(1, 0, 5.5);
		glEnd();

		glPopMatrix();
	}
}

void drawHUD()
{
	float left = 0.4 * leftShip->health;
	float right = 0.4 *  rightShip->health;
	float islandH = 0.04 * island->health;
	if (island->health > 0)
	{
		glColor4ub(255, 255, 0, 255);
		glBegin(GL_POLYGON);
		glVertex3f(1, 19, 9.5);
		glVertex3f(1, 18, 9.5);
		glVertex3f(1 + islandH, 18, 9.5);
		glVertex3f(1 + islandH, 19, 9.5);
		glEnd();
	}
	if (rightShip->health > 0)
	{
		glColor4ub(0, 0, 255, 255);
		glBegin(GL_POLYGON);
		glVertex3f(1, 17, 9.5);
		glVertex3f(1, 16, 9.5);
		glVertex3f(1 + right, 16, 9.5);
		glVertex3f(1 + right, 17, 9.5);
		glEnd();
	}
	if (leftShip->health > 0)
	{
		glColor4ub(255, 0, 0, 255);
		glBegin(GL_POLYGON);
		glVertex3f(1, 15, 9.5);
		glVertex3f(1, 14, 9.5);
		glVertex3f(1 + left, 14, 9.5);
		glVertex3f(1 + left, 15, 9.5);
		glEnd();
	}
}

void drawIslandVictory()
{
	std::string victoryText = "The island has been destroyed";

	glColor4f(255, 0, 0, 255);
	glRasterPos3f(10, 18, 10);

	for (int i = 0; i < (int) victoryText.size(); ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, victoryText[i]);
	}
	running = false;
}

void drawShipVictor()
{
	std::string victoryText = "The island has been defended";

	glColor4f(255, 0, 0, 255);
	glRasterPos3f(10, 18, 10);

	for (int i = 0; i < (int) victoryText.size(); ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, victoryText[i]);
	}
	running = false;
}

void drawOSD()
{
	std::stringstream numberFixer;
	numberFixer << std::fixed << std::setprecision(2) << curFrameRate;
	std::string frameRate = "Frame Rate: " + numberFixer.str() + " FPS";
	numberFixer.str(std::string());
	numberFixer << std::fixed << std::setprecision(2) << (1 / curFrameRate * 1000) * 100;
	std::string	frameTime = "Frame Time: "+ numberFixer.str() +" MS",
		teselation = "Teselation: "+ std::to_string((int)resolution);

	//Frame rate
	glColor4ub(0, 0, 0, 255);
	glRasterPos3f(14, 18, 10);
	for (int i = 0; i < (int)frameRate.size(); ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, frameRate[i]);
	}

	//Frame Time
	glRasterPos3f(14, 17, 10);
	for (int i = 0; i < (int)frameTime.size(); ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, frameTime[i]);
	}

	//Teselation
	glRasterPos3f(14, 16, 10);
	for (int i = 0; i < (int)teselation.size(); ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, teselation[i]);
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//Checks for wireframe mode
	if (wireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	//Island Loses
	if (island->health <= 0)
	{
		drawShipVictor();
	}
	//Island Wins
	if (leftShip->health <= 0 &&
		rightShip->health <= 0)
	{
		drawShipVictor();
	}
	//Draws Health Bars
	drawHUD();

	//Draws OSD
	if (OSD)
	{
		drawOSD();
	}

	//Draws Island
	island->draw(resolution);
	if (leftShip->health > 0)
	{
		//Draws the left ship
		drawShip(leftShip.get(), sinWave(WAVE_AMPLITUDE, WAVE_FREQUENCY, WAVE_HEIGHT, leftShip->curX), diffSinWave(WAVE_AMPLITUDE, WAVE_FREQUENCY, WAVE_HEIGHT, leftShip->curX));
	}
	if (rightShip->health > 0)
	{
		//Draws the right ship
		drawShip(rightShip.get(), sinWave(WAVE_AMPLITUDE, WAVE_FREQUENCY, WAVE_HEIGHT, rightShip->curX), diffSinWave(WAVE_AMPLITUDE, WAVE_FREQUENCY, WAVE_HEIGHT, rightShip->curX));
	}
	if (normals)
	{
		drawNormals();
	}
	if (tangents)
	{
		drawTangents();
	}
	drawWater(WAVE_AMPLITUDE, WAVE_FREQUENCY, WAVE_HEIGHT, resolution);

	if (glGetError())
	{
		std::cout << "GL Error\n";
		std::cout << gluErrorString(glGetError()) << std::endl;
	}

	frames++;

	glutSwapBuffers();
}

/*******************************
		Draw Methods End
********************************/

//Read user input
void keyboard(unsigned char key, int x, int y)
{
	if (running)
	{
		leftShip->checkInput(key, curFrameRate);
		rightShip->checkInput(key, curFrameRate);
		island->checkInput(key);
	}
	switch (key)
	{
		case '+': //Increases Teselation
			resolution *= 2;
			break;
		case '-': //Decreses Teselation
			if(resolution > 1)
				resolution /= 2;
			break;
		case '?':
		case '/': //Show Debug Info
			OSD = !OSD;
			break;
		case 'g': //Pause/Play
			running = !running;
			break;
		case 'n': //Show Normals
			normals = !normals;
			break;
		case 't': //Show Tangents
			tangents = !tangents;
			break;
		case 'w': //Show Wire Frame
			wireFrame = !wireFrame;
			break;
		case ESCAPE_KEY: //Quits game
			exit(EXIT_SUCCESS);
			break;
		default:
			break;
	}
}

/*******************************************
	Collision Detection Methods Start
********************************************/
bool islandCollisionDetection(Island *i, CannonBall* b)
{
	//Radius 1 + Radius 2
	float radVar = (i->collisionRadius + b->radius);
	radVar *= radVar;

	//root (x1 - x2)^2 + (y1 - y2)^2
	float distX = (i->midPoint - b->curX);
	distX *= distX;
	float distY = (i->height / 2 - b->curY);
	distY *= distY;
	float dist = distX + distY;

	if (radVar >= dist)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool shipCollisionDetection(Ship *s, CannonBall* b)
{
	//Radius 1 + Radius 2
	float radVar = (s->radius + b->radius);
	radVar *= radVar;

	//root (x1 - x2)^2 + (y1 - y2)^2
	float distX = (s->curX - b->curX);
	distX *= distX;
	float distY = (s->curY - b->curY);
	distY *= distY;
	float dist = distX + distY;

	if (radVar >= dist)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool shieldCollisionDetection(Shield *s, CannonBall* b)
{
	//Radius 1 + Radius 2
	float radVar = (s->getRadius() + b->radius);
	radVar *= radVar;

	//root (x1 - x2)^2 + (y1 - y2)^2
	float distX = (s->getX() - b->curX);
	distX *= distX;
	float distY = (s->getY() - b->curY);
	distY *= distY;
	float dist = distX + distY;

	if (radVar >= dist)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool waveCollisionDetection(CannonBall* b)
{
	if (b->curY <=
		sinWave(WAVE_AMPLITUDE, WAVE_FREQUENCY, WAVE_HEIGHT, b->curX))
	{
		return true;
	}
	return false;
}

void collisionDetection()
{
	//Attacks by left ship
	for (int i = 0; i < (int)leftShip->cannonBalls.size(); i++)
	{
		//Shield Collision With Left Ship
		if (rightShip->shield != nullptr)
		{
			if (shieldCollisionDetection(rightShip->shield.get(), leftShip->cannonBalls[i].get()))
			{
				leftShip->cannonBalls.erase(leftShip->cannonBalls.begin() + i);
				i--;
				continue;
			}
		}
		//Collision With Island
		if (islandCollisionDetection(island.get(), leftShip->cannonBalls[i].get()))
		{
			island->health--;
			leftShip->cannonBalls.erase(leftShip->cannonBalls.begin() + i);
			i--;
			continue;
		}
		//Collision With Water
		if (waveCollisionDetection(leftShip->cannonBalls[i].get()))
		{
			leftShip->cannonBalls.erase(leftShip->cannonBalls.begin() + i);
			i--;
			continue;
		}
		//Collision with Right Ship
		if (rightShip->health > 0)
		{
			if (shipCollisionDetection(rightShip.get(), leftShip->cannonBalls[i].get()))
			{
				rightShip->health--;
				leftShip->cannonBalls.erase(leftShip->cannonBalls.begin() + i);
				i--;
			}
		}
	}

	//Attacks by Right ship
	for (int i = 0; i < (int)rightShip->cannonBalls.size(); i++)
	{
		//Shield Collision With Left Ship
		if (leftShip->shield != nullptr)
		{
			if (shieldCollisionDetection(leftShip->shield.get(), rightShip->cannonBalls[i].get()))
			{
				rightShip->cannonBalls.erase(rightShip->cannonBalls.begin() + i);
				i--;
				continue;
			}
		}
		//Collision With Island
		if (islandCollisionDetection(island.get(), rightShip->cannonBalls[i].get()))
		{
			island->health--;
			rightShip->cannonBalls.erase(rightShip->cannonBalls.begin() + i);
			i--;
			continue;
		}
		//Collision With Water
		if (waveCollisionDetection(rightShip->cannonBalls[i].get()))
		{
			rightShip->cannonBalls.erase(rightShip->cannonBalls.begin() + i);
			i--;
			continue;
		}
		//Collision With Left Ship
		if (leftShip->health > 0)
		{
			if (shipCollisionDetection(leftShip.get(), rightShip->cannonBalls[i].get()))
			{
				leftShip->health--;
				rightShip->cannonBalls.erase(rightShip->cannonBalls.begin() + i);
				i--;
			}
		}
	}
	//Attacks by island
	for (int i = 0; i < (int)island->cannonBalls.size(); i++)
	{
		//Shield Collision With Left Ship
		if (leftShip->shield != nullptr)
		{
			if (shieldCollisionDetection(leftShip->shield.get(), island->cannonBalls[i].get()))
			{
				island->cannonBalls.erase(island->cannonBalls.begin() + i);
				i--;
				continue;
			}
		}

		//Shield Collision With Right Ship
		if (rightShip->shield != nullptr)
		{
			if (shieldCollisionDetection(rightShip->shield.get(), island->cannonBalls[i].get()))
			{
				island->cannonBalls.erase(island->cannonBalls.begin() + i);
				i--;
				continue;
			}
		}
		//Collision With Water
		if (waveCollisionDetection(island->cannonBalls[i].get()))
		{
			island->cannonBalls.erase(island->cannonBalls.begin() + i);
			i--;
			continue;
		}
		//Shield Collision With Left Ship
		if (leftShip->health > 0)
		{
			if (shipCollisionDetection(leftShip.get(), island->cannonBalls[i].get()))
			{
				leftShip->health--;
				island->cannonBalls.erase(island->cannonBalls.begin() + i);
				i--;
				continue;
			}
		}
		// Shield Collision With Right Ship
		if (rightShip->health > 0)
		{
			if (shipCollisionDetection(rightShip.get(), island->cannonBalls[i].get()))
			{
				rightShip->health--;
				island->cannonBalls.erase(island->cannonBalls.begin() + i);
				i--;
			}
		}
	}
}

/*******************************************
	Collision Detection Methods End
********************************************/

//Updates the gamestate
void idle()
{
	static float timeLast = 0;
	static float lastSecond = 0;
	float time, changeTime;
	time = glutGet(GLUT_ELAPSED_TIME);
	time /= 1000;
	changeTime = time - timeLast;
	lastSecond += changeTime;
	if (running)
	{
		//Update Ships
		leftShip->idle(changeTime);
		rightShip->idle(changeTime);

		//Update Island
		island->idle(changeTime);
		//Update Water
		waveMove(changeTime);
	}
	if (lastSecond >= 1)
	{
		calcFrameRate(lastSecond);
		lastSecond -= 1;
	}

	//Check for collision
	collisionDetection();

	timeLast = time;
	glutPostRedisplay();
}

//Initialise Game Variables
void init()
{
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, MAX_WIDTH, 0, MAX_HEIGHT, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);

	//Initial Wave Displacement
	waveDisplacement = 0.0;

	//Initial Resolution
	resolution = 2048;

	//Init Ships, sets their scale, orientation (positive or negative), position on the X axis health and boundaries
	rightShip = std::make_unique<Ship>((float)BOAT_SCALE, (float) 1.0, (float)15.0, SHIP_HEALTH, (float)ISLAND_RIGHT + 1, (float)19);
	leftShip = std::make_unique<Ship>((float)BOAT_SCALE, (float)-1.0, (float)5.0, SHIP_HEALTH, (float)1, (float)ISLAND_LEFT - 1);

	//Set Color
	rightShip->setColor(0, 0, 255, 255);
	leftShip->setColor(255, 0, 0, 255);

	//Set Controls
	rightShip->setShipControls('l', 'j', 'o', 'O', 'i', 'u');
	leftShip->setShipControls('d', 'a', 'q', 'Q', 'e', 'r');

	//Init Island
	island = std::make_unique<Island>((float)ISLAND_HEIGHT, (float)ISLAND_LEFT, (float)ISLAND_RIGHT, ISLAND_HEALTH);

	//Arbitary Initial Framerate
	curFrameRate = 400;
	frames = 1;
}

//Starts the game
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("I3D A1");

	init();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(255, 255, 255, 1);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	return EXIT_SUCCESS;
}