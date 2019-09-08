#include "CannonBall.h"

CannonBall::CannonBall(float startX, float startY, float velocity, float angle, float radius)
	: curX(startX), curY(startY), angle(angle), radius(radius)
{
	velocityX = (velocity * cos(angle));
	velocityY = (velocity * sin(angle));
}



CannonBall::~CannonBall()
{
}

void CannonBall::draw(float resolution)
{
	glPushMatrix();
	glTranslatef(curX, curY, 0);
	glColor4ub(0, 0, 0, 255);
	glBegin(GL_POLYGON);
	//draws a circle
	for (int i = 0; i < 360; i++)
	{
		float x = DEG2RAD * i;
		glVertex3f(cos(x) * radius, sin(x) * radius, 0);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//Refactor
	//Remove Magic numbers
	float stepSize = 0.005;
	float xStep = 0;
	float dist = 1;
	int i = 0;
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	while (dist > 0) 
	{
		i++;
		xStep += stepSize;
		dist = (velocityY)*xStep + 0.5*GRAVITY*xStep*xStep + curY;
		//Z value to hide behind water
		glVertex3f(velocityX*xStep + curX, dist, -9);
	}
	glEnd();
	
	glPopMatrix();

}

void CannonBall::idle(float changeTime)
{
	//Update X and Y
	
	curX += (velocityX * changeTime);
	curY +=  (velocityY * changeTime);
	velocityY += GRAVITY * changeTime;
}
