#include "Shield.h"



Shield::Shield(float startX, float startY) : curX(startX), curY(startY + HEIGHT_COVER), radius(START_RADIUS)
{

}


Shield::~Shield()
{
}

//Draws shield
void Shield::draw(float resolution)
{
	glPushMatrix();
	glTranslatef(curX, curY, 0);
	glColor4ub(0, 0, 0, 255);
	glBegin(GL_LINE_LOOP);
	//draws a circle
	for (int i = 0; i < 360; i++)
	{
		float x = DEG2RAD * i;
		glVertex3f(cos(x) * radius, sin(x) * radius, 9);
	}
	glEnd();
	glPopMatrix();
}

const float & Shield::getRadius()
{
	return radius;
}

const float & Shield::getX()
{
	return curX;
}

const float & Shield::getY()
{
	return curY;
}

//Updates radius
void Shield::idle(float changeTime)
{
	radius += radius * changeTime;
}
