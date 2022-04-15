#include <iostream>
#include <cmath>
#include "cell.h"
#include "stdlib.h"

cell::cell(unsigned int Seed) {
	seed = Seed;
	x = 0;
	y = 0;

	xf = 0;
	yf = 0;
	
	srand(seed);
	calcTrunc();

	cell* cellsAround = new cell[9];
	cell* cellsAroundNext = new cell[9];

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

	colorR = 255;
	colorG = 255;
	colorB = 255;

	typeID = 1;

	double* otherInitialVComp = new double[2];
	double* ourInitalVComp = new double[2];

	double* FinalVelocity = new double[2];
	double* otherFinalVelocity = new double[2];

}

cell::cell(unsigned int Seed, int X, int Y, int ColorR, int ColorG, int ColorB, int ID, double Mass)
{

	typeID = ID;

	x = X;
	y = Y;

	xf = 0;
	yf = 0;

	seed = hypot(x * Seed, y * Seed);

	srand(seed);
	calcTrunc();

	cell* cellsAround = new cell[9];
	cell* cellsAroundNext = new cell[9];

	forceComponentVector = new double[2];
	forceVector = new double[2];
	velocityComponentVector = new double[2];
	velocityVector = new double[2];
	thermal = new double[2];
	temperature = 295.0;

	FinalVelocity = new double[2];
	otherFinalVelocity = new double[2];

	colorR = ColorR;
	colorG = ColorG;
	colorB = ColorB;

	//place holders
	thermalConductivity = 1;
	mass = Mass;
	genThermal();

	double* otherInitialVComp = new double[2];
	double* ourInitalVComp = new double[2];
	double* FinalVelocity = new double[2];
	double* otherFinalVelocity = new double[2];

}

cell::~cell()
{

	delete[] cellsAround;
	delete[] cellsAroundNext;

	delete[] forceComponentVector;
	delete[] forceVector;
	delete[] velocityComponentVector;
	delete[] velocityVector;
	delete[] thermal;
	delete[] FinalVelocity;
	delete[] otherFinalVelocity;


	delete[] otherInitialVComp;
	delete[] ourInitalVComp;

	delete[] FinalVelocity;
	delete[] otherFinalVelocity;


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
	if (xf - x > 0)
		return 3;
	return 2;
}

int cell::getDirY() {
	if (yf - y > 0)
		return 0;
	return 1;

}

sf::Vector2f cell::getPosition()
{
	return sf::Vector2f(x_t, y_t);
}

sf::Vector2f cell::getFuturePosition()
{
	return sf::Vector2f(x, y);
}

double cell::getTemp() {
	return temperature;
}

void cell::setNeighbor(int I, cell* pointer) {

	cellsAround[I] = cellsAroundNext[I];
	cellsAroundNext[I] = *pointer;

}

void cell::setColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A)
{

	color.r = R;
	color.g = G;
	color.b = B;
	color.a = A;

}

int cell::getColorR()
{
	return colorR;
}

int cell::getColorG()
{
	return colorG;
}

int cell::getColorB()
{
	return colorB;
}



void cell::update(double timeStep, double* g) {
	timeScale = timeStep;
	addForceVector(g);

	sumVectors();
	calcMagnitudeVectors();
	xf += velocityComponentVector[0] * timeScale;
	yf += velocityComponentVector[1] * timeScale;

	cell* target = &cellsAround[int(xf-x) + (int(yf - y) * 3)];
	if (target && target != this)
		impact(target);
	else
		move();
}

void cell::move() {
	x = xf;
	y = yf;
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
	forceComponentVector[0] = 0.0;
	forceComponentVector[1] = 0.0;

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

//vestigule
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