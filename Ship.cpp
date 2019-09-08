#include "Ship.h"



Ship::Ship(float scale, float orientation, float startX, int shipHealth, float boundLeft, float boundRight)
	: scale(scale), orientation(orientation), cannonAngle(SHIP_STARTING_CANNON_ANGLE), curX(startX), curBoatRotation(1),
	health(shipHealth), radius(RADIUS), boundLeft(boundLeft), boundRight(boundRight), maxCannonBalls(MAX_CANNON_BALLS),
	red(0), green(0), blue(0), opacity(255), coolingDown(false), shieldCoolingDown(false), cannonCooldownTimer(0), shieldTimer(0)
{	
	
}

void Ship::setColor(int r, int g, int b, int o)
{
	red = r;
	green = g;
	blue = b;
	opacity = o;
}

float Ship::getRealCannonAngle()
{
	if (orientation > 0)
	{
		realCannonAngle = curBoatRotation + 180 - cannonAngle;
	}
	else
	{
		realCannonAngle = curBoatRotation + cannonAngle;
	}
	return realCannonAngle;
}



void Ship::checkInput(unsigned char input, float curFrameRate)
{
	try
	{
		if (input == controls[0]) //Move Right
		{
			if (this->curX + 0.1< boundRight)
			{
				orientation = LEFT_ORIENTATION;
				this->curX += 0.1;
				
			}
		}
		if (input == controls[1])//Move Left
		{
			if (this->curX + 0.1> boundLeft)
			{
				orientation = RIGHT_ORIENTATION;
				this->curX -= 0.1;
				
			}
		}
		if (input == controls[2])//Moves Cannon up
		{
			if (this->cannonAngle < 90)
			{
				this->cannonAngle += 1;
			}
		}
		if (input == controls[3])//Move Cannon Down
		{
			if (this->cannonAngle > 0)
			{
				this->cannonAngle -= 1;
			}
		}
		if (input == controls[4])//Fire Cannon
		{
			if ((int)cannonBalls.size() < maxCannonBalls && !coolingDown)
			{
				cannonBalls.push_back(std::make_unique<CannonBall>(cannonX, cannonY, SHIP_CANNON_VELOCITY, realCannonAngle*DEG2RAD, SHIP_CANNON_BALL_RADIUS));
				coolingDown = true;
			}
		}
		if (input == controls[5])//Deploy Shield
		{
			if (shield == nullptr && !shieldCoolingDown)
			{
				shield = std::make_unique<Shield>(curX, curY);
			}
		}
		else
		{
			return;
		}
	}
	catch(std::exception e)
	{
		std::cout << e.what();
	}
}


void Ship::drawShip(float TranslationY, float boatRotation, float resolution)
{
	curY = TranslationY;
	curBoatRotation = boatRotation;
	realCannonAngle = getRealCannonAngle();

	glPushMatrix();

	glTranslatef(curX, TranslationY, 0);
	glScalef(scale, scale, scale);
	glRotatef(boatRotation, 0, 0, 1);
	glColor4ub(red, green, blue, opacity);

	//Draw Hull
	glBegin(GL_POLYGON);
	glVertex3f(HULL_BOTTOM_RIGHT_X, HULL_BOTTOM, 0.5);
	glVertex3f(HULL_TOP_RIGHT_X, HULL_TOP, 0.5);
	glVertex3f(HULL_TOP_LEFT_X, HULL_TOP, 0.5);
	glVertex3f(HULL_BOTTOM_LEFT_X, HULL_BOTTOM, 0.5);
	glEnd();

	//Draw Roof
	glBegin(GL_POLYGON);
	glVertex3f(ROOF_LEFT_X * orientation, ROOF_BOTTOM, 0.5);
	glVertex3f(ROOF_LEFT_X * orientation, ROOF_TOP, 0.5);
	glVertex3f(ROOF_RIGHT_X * orientation, ROOF_TOP, 0.5);
	glVertex3f(ROOF_RIGHT_X * orientation, ROOF_BOTTOM, 0.5);	
	glEnd();
	
	
	glTranslatef(0, 0.7, 0);
	glRotatef(-1 * orientation * cannonAngle, 0, 0, 1);
	
	//Draws Cannon
	glBegin(GL_POLYGON);
	glVertex3f(CANNON_LEFT_X, CANNON_BOTTOM, 0.5);
	glVertex3f(CANNON_RIGHT_X * orientation, CANNON_BOTTOM, 0.5);
	glVertex3f(CANNON_RIGHT_X * orientation, CANNON_TOP, 0.5);
	glVertex3f(CANNON_LEFT_X, CANNON_TOP, 0.5);
	glEnd();

	/*
		In this section we
		Take the transformations applied to the draw matrix and saves them to a array
		then we use that array to find the position and angle of the cannon
	*/
	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

	float xPos = matrix[0] * (CANNON_RIGHT_X * orientation) + matrix[4] * (CANNON_TOP) + matrix[8] + matrix[12];
	float yPos = matrix[1] * (CANNON_RIGHT_X * orientation) + matrix[5] * (CANNON_TOP) + matrix[9] + matrix[13];
	float mod = matrix[3] * (CANNON_RIGHT_X * orientation) + matrix[7] * (CANNON_TOP) + matrix[11] + matrix[15];

	cannonX = xPos / mod;
	cannonY = yPos / mod;

	glPopMatrix();
	
	//draw cannon balls
	for (int i = 0; i < (int) cannonBalls.size(); i++)
	{
		cannonBalls[i]->draw(resolution);
	}

	//draws the shield
	if (shield != nullptr)
	{
		shield->draw(resolution);
	}
}

//Updates the boat
void Ship::idle(float changeTime)
{
	//Updates Cooldown Timer
	if (coolingDown)
	{
		cannonCooldownTimer += changeTime;
		//Checks if firing can be re-enabled
		if (cannonCooldownTimer >= SHIP_CANNON_COOLDOWN_TIME)
		{
			coolingDown = false;
			cannonCooldownTimer = 0;
		}
	}

	//Update shield cooldown timer
	if (shieldCoolingDown)
	{
		shieldTimer += changeTime;
		//Checks if shield can be used again
		if (shieldTimer >= SHIP_SHIELD_COOLDOWN)
		{
			shieldCoolingDown = false;
			shieldTimer = 0;
		}
	}

	//Update cannon balls
	for (int i = 0; i < (int) cannonBalls.size(); i++)
	{
		cannonBalls[i]->idle(changeTime);
	}

	//Updates shield if it is currently deployed
	if (shield != nullptr)
	{
		shield->idle(changeTime);
		if (shield->getRadius() > 3)
		{
			shield.reset();
			shieldCoolingDown = true;
		}
	}
}

//Sets the control scheme for the boat
void Ship::setShipControls(char forward, char backward, char aimUp, char aimDown, char fire, char shield)
{
	controls.push_back(forward);
	controls.push_back(backward);
	controls.push_back(aimUp);
	controls.push_back(aimDown);
	controls.push_back(fire);
	controls.push_back(shield);
}


Ship::~Ship()
{
}
