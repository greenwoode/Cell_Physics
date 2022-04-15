#include <iostream>
#include <cmath>
#include "cell.h"
#include "stdlib.h"

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

	FinalVelocity = new double[2];
	otherFinalVelocity = new double[2];

	//place holders
	thermalConductivity = 1;
	mass = 10;
	meltingPoint = 1000;

	genThermal();

	color = sf::Color(255,255,255,255);

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

	FinalVelocity = new double[2];
	otherFinalVelocity = new double[2];

	//place holders
	thermalConductivity = 1;
	mass = 10;
	genThermal();

	color = Color;

}

bool cell::addForceVector(double* newComVector) {

	if(typeID)
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

double cell::getMass()
{
	return mass;
}

int cell::getDirX() {
	if (x - oldX > 0)
		return 3;
	return 2;
}

int cell::getDirY() {
	if (y - oldY > 0)
		return 0;
	return 1;

	return y - oldY;
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

void cell::update(double timeStep) {
	timeScale = timeStep;
	try {

		// Temp calucation + state change
		// Temp check 273.1
		//	 Type check
		//		water => temp check (temp == 273.1)
							//=> state change


		// Decide which neighbor to check
		cell* destinationX = &cellsAround[getDirX()];
		cell* destinationY = &cellsAround[getDirY()];

		// moving stuff
		// if theres no neighbors just move
		// if there is neighbors check which direction
		if (destinationX == nullptr && destinationY == nullptr) {
			move(timeScale);		
		} else {
			if (destinationY != nullptr && destinationY != nullptr) {
				move(destinationX, destinationY, timeScale);
			} else if (destinationY == nullptr) {
				move(destinationX,timeScale);
			} else if (destinationY == nullptr) {
				move(destinationY, timeScale);
			}
		}
	} catch (const std::exception& e) {
		std::cout << e.what();
	}

}

void cell::move(double timeScale) {
	oldX = x;
	oldY = y;
	x += velocityComponentVector[0] * timeScale;
	y += velocityComponentVector[1] * timeScale;
}

void cell::move(cell *otherCell, double timeScale) {
	impact(otherCell);
	otherCell->move(timeScale);
	oldX = x;
	oldY = y;
	x += velocityComponentVector[0] * timeScale;
	y += velocityComponentVector[1] * timeScale;
}

void cell::move(cell *otherCellX, cell *otherCellY, double timeScale){
	impact(otherCellX);
	impact(otherCellY);
	otherCellX->move(timeScale);
	otherCellY->move(timeScale);
	oldX = x;
	oldY = y;
	x += velocityComponentVector[0] * timeScale;
	y += velocityComponentVector[1] * timeScale;
}

void cell::impact(cell *otherCell) {
	/*
		Equation:
			U1 = ((m1 - m2)(u1 - u2))/(m1 + m2) + u2
			U2 = (2m1(u1 - u2))/(m1 + m2) + u2

			u = inital velocity
			U = final velocity

		this need to be done in both axis
	*/

	otherMass = (*otherCell).getMass();
	otherInitialVComp = (*otherCell).getVelocityComponentVector();
	ourInitalVComp = getVelocityComponentVector();
	totalMass = mass + otherMass;

	// U1x and U2x
	FinalVelocity[0] = (((mass - otherMass)*(ourInitalVComp[0] - otherInitialVComp[0])) / (mass + otherMass)) + otherInitialVComp[0];
	otherFinalVelocity[0] = ((2 * mass*(ourInitalVComp[0] - otherInitialVComp[0])) / (totalMass)) + otherInitialVComp[0];

	// U1y and U2y
	FinalVelocity[1] = (((mass - otherMass)*(ourInitalVComp[1] - otherInitialVComp[1])) / (mass + otherMass)) + otherInitialVComp[1];
	otherFinalVelocity[1] = ((2 * mass*(ourInitalVComp[1] - otherInitialVComp[1])) / (totalMass)) + otherInitialVComp[1];

	// add final velocity to our cell
	addForceVector(FinalVelocity);

	// add final velocity to other cell
	(*otherCell).addForceVector(otherFinalVelocity);

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

//Jon's functions

//Updates the material
void cell::updateMaterial(int id) {
	typeID = id;
}

//LOGIC to add to Cell Update

void cell::updateInformation() {
	//Temporary for sake of no errors
	//int freeze;
	//int meltingPoint;
	//int temperature;
	//int meltIntoID;

	//if (temperature > meltingPoint) {
	//	updateMaterial(meltIntoID);
	//}

}