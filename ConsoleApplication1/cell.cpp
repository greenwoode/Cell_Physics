#include "cell.h"

cell::cell(unsigned int Seed) {
	seed = Seed;
	x = 0;
	y = 0;
	
	srand(seed);
	calcTrunc();

	cell* cellsAround = new cell[4];
	cell* cellsAroundNext = new cell[4];

	forceComponentVector = new double[2];
	forceVector = new double[2];
	velocityComponentVector = new double[2];
	velocityVector = new double[2];
	thermal = new double[2];
	temperature = 295.0;

	//place holders
	thermalConductivity = 1;
	mass = 10;

	genThermal();

	setColor(255, 255, 255);

}

cell::cell(unsigned int Seed, int X, int Y, sf::Color Color)
{

	x = X;
	y = Y;

	seed = hypot(x * Seed, y * Seed);

	srand(seed);
	calcTrunc();

	cell* cellsAround = new cell[4];
	cell* cellsAroundNext = new cell[4];

	forceComponentVector = new double[2];
	forceVector = new double[2];
	velocityComponentVector = new double[2];
	velocityVector = new double[2];
	thermal = new double[2];
	temperature = 295.0;

	//place holders
	thermalConductivity = 1;
	mass = 10;

	genThermal();

	color = Color;

}

bool cell::addForceVector(double* newComVector) {

	forces.push(newComVector);

	return true;
}

double* cell::getVelocityVector(bool update)
{

	if (update) {
		sumVectors();
		calculateVelocity();
		calcMagnitudeVectors();
	}

	return velocityVector;
}

double * cell::getVelocityComponentVector()
{
	return velocityComponentVector;
}

sf::Vector2f cell::getPosition()
{
	return sf::Vector2f(x_t, y_t);
}

sf::Vector2f cell::getExactPosition()
{
	return sf::Vector2f();
}

sf::Color cell::getColor()
{
	return color;
}

double cell::getTemp() {
	return temperature;
}

void cell::setNeighbor(int Dir, cell* pointer) {

	cellsAround[Dir] = cellsAroundNext[Dir];
	cellsAroundNext[Dir] = *pointer;

}

void cell::setColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A)
{

	color.r = R;
	color.g = G;
	color.b = B;
	color.a = A;

}

// TODO: 1. Implement this
/*
	Step 1: check if velocity is != 0
			If yes then step 2
	Step 2: check if there are neighbor next to it
					If yes then call impact
					else nothing happend
*/
void cell::move()
{
	// Check velocity
	// If both are zero then they are not moving, in theory would stop it from moving
	if (velocityComponentVector[0] != 0 || velocityComponentVector[1] != 0) {
		// Check neighbors
		if (velocityComponentVector[0] > 0 && velocityComponentVector[1] == 0) {
			// Goin right => check right
		} else if (velocityComponentVector[0] == 0 && velocityComponentVector[1] > 0) {
			// goin up => check top
		} else if (velocityComponentVector[0] < 0 && velocityComponentVector[1] == 0) {
			// goin left => check left neighbor
		} else if (velocityComponentVector[0] == 0 && velocityComponentVector[1] < 0) {
			// goin down => check bottom neighbor
		} else if (velocityComponentVector[0] > 0 && velocityComponentVector[1] > 0) {
			// goin up right => check top and right
		} else if (velocityComponentVector[0] < 0 && velocityComponentVector[1] > 0) {
			// goin up left => check up and left
		} else if (velocityComponentVector[0] > 0 && velocityComponentVector[1] < 0) {
			// goin down right => check bottom and right
		} else if (velocityComponentVector[0] < 0 && velocityComponentVector[1] < 0) {
			// goin down left => check bottom and left
		}

	}
}

// TODO: Check if this is needed
bool cell::collisionCheck(cell otherCell)
{
	sf::Vector2f otherPos = otherCell.getExactPosition();
	sf::Vector2f ourPos = getExactPosition();

	if (ourPos == otherPos) {
		impact(otherCell);
		return true;
	}
	return false;
}

// https://sci-hub.se/https://aapt.scitation.org/doi/10.1119/1.2165433#:~:text=These%20formulas%20are%20derived%20from,1m1%20%2B%20m2
void cell::impact(cell otherCell) {
	/*
		Equation:
			U1 = ((m1 - m2)(u1 - u2))/(m1 + m2) + u2
			U2 = (2m1(u1 - u2))/(m1 + m2) + u2

			u = inital velocity
			U = final velocity

		this need to be done in both axis
	*/
	double otherMass = otherCell.mass;
	double* otherInitialVComp = otherCell.getVelocityComponentVector();
	double* ourInitalVComp = getVelocityComponentVector();
	double totalMass = mass + otherMass;

	double* FinalVelocity = new double[2];
	double* otherFinalVelocity = new double[2];

	// U1x and U2x
	FinalVelocity[0] = (((mass - otherMass)*(ourInitalVComp[0] - otherInitialVComp[0])) / (mass + otherMass)) + otherInitialVComp[0];
	otherFinalVelocity[0] = ((2 * mass*(ourInitalVComp[0] - otherInitialVComp[0])) / (totalMass)) + otherInitialVComp[0];

	// U1y and U2y
	FinalVelocity[0] = (((mass - otherMass)*(ourInitalVComp[0] - otherInitialVComp[0])) / (mass + otherMass)) + otherInitialVComp[0];
	otherFinalVelocity[0] = ((2 * mass*(ourInitalVComp[0] - otherInitialVComp[0])) / (totalMass)) + otherInitialVComp[0];

	// add final velocity to our cell
	addForceVector(FinalVelocity);

	// add final velocity to other cell
	otherCell.addForceVector(otherFinalVelocity);

}

//TODO
void cell::adjustTemperature()
{
	
	/*
	cellsAround[0] = *parentGrid.cellAt(this, x, y + 1);
	cellsAround[1] = *parentGrid.cellAt(this, x, y - 1);
	cellsAround[2] = *parentGrid.cellAt(this, x + 1, y);
	cellsAround[3] = *parentGrid.cellAt(this, x - 1, y);
	*/

	//TODO temp change
	double averageSurroundTemp = 0;
	for (int i = 0; i < 4; i++) {
		if (&cellsAround[i] != this) {
			averageSurroundTemp += cellsAround[i].getTemp();
		}
	}
	averageSurroundTemp /= 4.0;



}

void cell::genThermal()
{

	adjustTemperature();
	addForceVector(thermalInverse);

	thermal[0] = sqrt(temperature) / (1000.0*rand());
	thermal[1] = sqrt(temperature) / (1000.0*rand());

	addForceVector(thermal);

	thermalInverse[0] = -thermal[0];
	thermalInverse[1] = -thermal[1];


}

void cell::sumVectors()
{
	memset(forceComponentVector, 0, 2);

	for (int i = 0; i < forces.size(); i++) {
		double* tempVec = forces.front();
		forces.pop();
		velocityComponentVector[0] += tempVec[0];
		velocityComponentVector[1] += tempVec[1];
	}

}

void cell::calculateVelocity()
{

	velocityComponentVector[0] += forceComponentVector[0];
	velocityComponentVector[0] += forceComponentVector[1];

}

void cell::calcMagnitudeVectors()
{

	forceVector[0] = hypot(forceComponentVector[0], forceComponentVector[1]);
	forceVector[1] = atan2(forceComponentVector[1], forceComponentVector[0]);

	velocityVector[0] = hypot(velocityComponentVector[0], velocityComponentVector[1]);
	velocityVector[1] = atan2(velocityComponentVector[1], velocityComponentVector[0]);

}

void cell::calcTrunc()
{
	x_t = trunc(x);
	y_t = trunc(y);
}