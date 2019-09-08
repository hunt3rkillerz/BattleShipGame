#include "Island.h"



Island::Island(float height, float left, float right, int islandHealth) 
	: height(height), left(left), right(right), health(islandHealth),
	cannonAngle(STARTING_CANNON_ANGLE), coolingDown(false), cannonCooldownTimer(0), maxCannonBalls(5)
{
	//Sections out the island
	float diff = (right - left)/3;
	//Positions the cannon
	cannonLeft = left + diff;
	cannonRight = right - diff;

	//Finds mid point in the X axis
	midPoint = left + ( ((right - left) / 2));

	//2.5 is constant that seemed to help generate the best collision radius
	collisionRadius = height / 2.5;
}

void Island::draw(float resolution)
{
	glPushMatrix();
	//Yellow
	glColor4ub(0, 255, 255, 255);

	//Draw Island
	glBegin(GL_QUADS);
	glVertex3f(left, ISLAND_BOTTOM, 0.5);
	glVertex3f(right, ISLAND_BOTTOM, 0.5);
	glVertex3f(right, height, 0.5);
	glVertex3f(left, height, 0.5);
	glEnd();

	glColor4ub(255, 255, 0, 255);
	glTranslatef(midPoint, height, 0);
	glRotatef(cannonAngle, 0, 0, 1);

	//Draw Cannon
	glBegin(GL_POLYGON);
	glVertex3f(0, -0.5, 2);
	glVertex3f(0, 0.5, 2);
	glVertex3f(3, 0.5, 2);
	glVertex3f(3, -0.5, 2);
	glEnd();

	/*
		In this section we
		Take the transformations applied to the draw matrix and saves them to a array
		then we use that array to find the position and angle of the cannon
	*/
	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

	float xPos = matrix[0] * CANNON_HEIGHT + matrix[4] *  matrix[8] + matrix[12];
	float yPos = matrix[1] * CANNON_HEIGHT + matrix[5] * matrix[9] + matrix[13];
	float mod = matrix[3] * CANNON_HEIGHT + matrix[7] * matrix[11] + matrix[15];

	cannonX = xPos / mod;
	cannonY = yPos / mod;


	glPopMatrix();

	//Draw cannon balls
	for (int i = 0; i < (int) cannonBalls.size(); i++)
	{
		cannonBalls[i]->draw(resolution);
	}
}

void Island::idle(float changeTime)
{
	//Updates Cooldown Timer
	if (coolingDown)
	{
		cannonCooldownTimer += changeTime;
		//Checks if firing can be re-enabled
		if (cannonCooldownTimer >= ISLAND_CANNON_COOLDOWN_TIME)
		{
			coolingDown = false;
			cannonCooldownTimer = 0;
		}
	}

	//Update cannon balls
	for (int i = 0; i < (int) cannonBalls.size(); i++)
	{
		cannonBalls[i]->idle(changeTime);
	}
}

void Island::checkInput(unsigned char input)
{
		if (input == 'f')//Aim Up
		{
			if (this->cannonAngle < 180)
			{
				this->cannonAngle += 1;
			}
		}
		if (input == 'h')//Aim Down
		{
			if (this->cannonAngle > 0)
			{
				this->cannonAngle -= 1;
			}
		}
		if (input == ' ' || input == '9')//Fire
		{
			if ((int) cannonBalls.size() < maxCannonBalls && !coolingDown)
			{

				cannonBalls.push_back(std::make_unique<CannonBall>(cannonX, cannonY, CANNON_VELOCITY, cannonAngle * DEG2RAD, 0.2));
				coolingDown = true;
			}
		}
}


Island::~Island()
{
}
