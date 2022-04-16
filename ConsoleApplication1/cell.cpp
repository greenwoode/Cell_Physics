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

	cellsAround = NULL;
	cellsAroundNext = NULL;

	forceComponentVector = new double[2];
	forceComponentVector[0] = 0.0;
	forceComponentVector[1] = 0.0;

	forceVector = new double[2];
	forceVector[0] = 0.0;
	forceVector[1] = 0.0;

	velocityComponentVector = new double[2];
	velocityComponentVector[0] = 0.0;
	velocityComponentVector[1] = 0.0;


	velocityVector = new double[2];
	velocityVector[0] = 0.0;
	velocityVector[1] = 0.0;

	//thermals = new double[2];
	//thermalsInverse = new double[2];
	//temperature = 295.0;

	FinalVelocity = new double[2];
	FinalVelocity[0] = 0.0;
	FinalVelocity[1] = 0.0;

	otherFinalVelocity = new double[2];
	otherFinalVelocity[0] = 0.0;
	otherFinalVelocity[1] = 0.0;

	//place holders
	//thermalsConductivity = 1;
	mass = 10;
	meltingPoint = 1000;

	//gen//thermals();

	colorR = 255;
	colorG = 255;
	colorB = 255;

	typeID = 1;

	otherInitialVComp = new double[2];
	otherInitialVComp[0] = 0.0;
	otherInitialVComp[1] = 0.0;

	ourInitalVComp = new double[2];
	ourInitalVComp[0] = 0.0;
	ourInitalVComp[1] = 0.0;

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

	cellsAround = new cell*[9];
	memset(cellsAround, 0, 9*sizeof(nullptr));
	cellsAroundNext = new cell*[9];
	memset(cellsAroundNext, 0, 9 * sizeof(nullptr));

	forceComponentVector = new double[2];
	forceComponentVector[0] = 0.0;
	forceComponentVector[1] = 0.0;

	forceVector = new double[2];
	forceVector[0] = 0.0;
	forceVector[1] = 0.0;

	velocityComponentVector = new double[2];
	velocityComponentVector[0] = 0.0;
	velocityComponentVector[1] = 0.0;


	velocityVector = new double[2];
	velocityVector[0] = 0.0;
	velocityVector[1] = 0.0;

	//thermals = new double[2];
	//thermalsInverse = new double[2];
	//temperature = 295.0;

	FinalVelocity = new double[2];
	FinalVelocity[0] = 0.0;
	FinalVelocity[1] = 0.0;

	otherFinalVelocity = new double[2];
	otherFinalVelocity[0] = 0.0;
	otherFinalVelocity[1] = 0.0;

	colorR = ColorR;
	colorG = ColorG;
	colorB = ColorB;

	//place holders
	//thermalConductivity = 1;
	mass = Mass;
	//genThermal();

	otherInitialVComp = new double[2];
	otherInitialVComp[0] = 0.0;
	otherInitialVComp[1] = 0.0;

	ourInitalVComp = new double[2];
	ourInitalVComp[0] = 0.0;
	ourInitalVComp[1] = 0.0;

}

cell::cell(const cell& that) {
	printf("copy constructor called\n");
	typeID = that.typeID;

	x = that.x;
	y = that.y;

	xf = 0;
	yf = 0;

	seed = that.seed;

	srand(seed);
	calcTrunc();

	cellsAround = new cell * [9];
	memset(cellsAround, 0, 9 * sizeof(nullptr));
	cellsAroundNext = new cell * [9];
	memset(cellsAroundNext, 0, 9 * sizeof(nullptr));

	forceComponentVector = new double[2];
	forceComponentVector[0] = that.forceComponentVector[0];
	forceComponentVector[1] = that.forceComponentVector[1];

	forceVector = new double[2];
	forceVector[0] = that.forceVector[0];
	forceVector[1] = that.forceVector[1];

	velocityComponentVector = new double[2];
	velocityComponentVector[0] = that.velocityComponentVector[0];
	velocityComponentVector[1] = that.velocityComponentVector[1];


	velocityVector = new double[2];
	velocityVector[0] = that.velocityVector[0];
	velocityVector[1] = that.velocityVector[1];

	//thermals = new double[2];
	//thermalsInverse = new double[2];
	//temperature = 295.0;

	FinalVelocity = new double[2];
	FinalVelocity[0] = that.FinalVelocity[0];
	FinalVelocity[1] = that.FinalVelocity[1];

	otherFinalVelocity = new double[2];
	otherFinalVelocity[0] = that.otherFinalVelocity[0];
	otherFinalVelocity[1] = that.otherFinalVelocity[1];

	colorR = that.colorR;
	colorG = that.colorG;
	colorB = that.colorB;

	//place holders
	//thermalsConductivity = that.thermalConductivity;
	mass = that.mass;
	//genThermal();

	otherInitialVComp = new double[2];
	otherInitialVComp[0] = that.otherInitialVComp[0];
	otherInitialVComp[1] = that.otherInitialVComp[1];

	ourInitalVComp = new double[2];
	ourInitalVComp[0] = that.ourInitalVComp[0];
	ourInitalVComp[1] = that.ourInitalVComp[1];
}

cell& cell::operator=(const cell& that) {
	printf("copy assignment called\n");

	if (this != &that) {

		if (!dummyCell) {
			delete[] cellsAround;
			delete[] cellsAroundNext;
		}

		delete[] forceComponentVector;
		delete[] forceVector;
		delete[] velocityComponentVector;
		delete[] velocityVector;
		//delete[] thermal;
		//delete[] thermalInverse;

		delete[] FinalVelocity;
		delete[] otherFinalVelocity;


		delete[] otherInitialVComp;
		delete[] ourInitalVComp;

		delete[] FinalVelocity;
		delete[] otherFinalVelocity;


		typeID = that.typeID;

		x = that.x;
		y = that.y;

		xf = 0;
		yf = 0;

		seed = that.seed;

		srand(seed);
		calcTrunc();

		cellsAround = new cell * [9];
		memset(cellsAround, 0, 9 * sizeof(nullptr));
		cellsAroundNext = new cell * [9];
		memset(cellsAroundNext, 0, 9 * sizeof(nullptr));

		forceComponentVector = new double[2];
		forceComponentVector[0] = that.forceComponentVector[0];
		forceComponentVector[1] = that.forceComponentVector[1];

		forceVector = new double[2];
		forceVector[0] = that.forceVector[0];
		forceVector[1] = that.forceVector[1];

		velocityComponentVector = new double[2];
		velocityComponentVector[0] = that.velocityComponentVector[0];
		velocityComponentVector[1] = that.velocityComponentVector[1];


		velocityVector = new double[2];
		velocityVector[0] = that.velocityVector[0];
		velocityVector[1] = that.velocityVector[1];

		//thermals = new double[2];
		//thermalsInverse = new double[2];
		//temperature = 295.0;

		FinalVelocity = new double[2];
		FinalVelocity[0] = that.FinalVelocity[0];
		FinalVelocity[1] = that.FinalVelocity[1];

		otherFinalVelocity = new double[2];
		otherFinalVelocity[0] = that.otherFinalVelocity[0];
		otherFinalVelocity[1] = that.otherFinalVelocity[1];

		colorR = that.colorR;
		colorG = that.colorG;
		colorB = that.colorB;

		//place holders
		//thermalConductivity = that.thermalConductivity;
		mass = that.mass;
		//genThermal();

		otherInitialVComp = new double[2];
		otherInitialVComp[0] = that.otherInitialVComp[0];
		otherInitialVComp[1] = that.otherInitialVComp[1];

		ourInitalVComp = new double[2];
		ourInitalVComp[0] = that.ourInitalVComp[0];
		ourInitalVComp[1] = that.ourInitalVComp[1];


	}
	return *this;
}

cell::~cell()
{
	if (!dummyCell) {
		delete[] cellsAround;
		delete[] cellsAroundNext;
	}

	delete[] forceComponentVector;
	delete[] forceVector;
	delete[] velocityComponentVector;
	delete[] velocityVector;
	//delete[] thermal;
	//delete[] thermalInverse;
	delete[] FinalVelocity;
	delete[] otherFinalVelocity;


	delete[] otherInitialVComp;
	delete[] ourInitalVComp;

	//delete[] FinalVelocity;
	//delete[] otherFinalVelocity;


}

bool cell::addForceVector(double* newComVector) {

	double* temp = new double[2];
	temp[0] = newComVector[0];
	temp[1] = newComVector[1];

	if(typeID)
		forces.push(temp);

	return true;
}

double* cell::getVelocityVector(bool update)
{

	if (update) {
		sumVectors();
		calculateVelocity();
		calcMagnitudeVectors();
	}
	calcMagnitudeVectors();
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

//vestigual
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
	return sf::Vector2f(x, y);
}

sf::Vector2f cell::getFuturePosition()
{
	return sf::Vector2f(xf, yf);
}

double cell::getTemp() {
	return temperature;
}

void cell::setNeighbor(int I, cell* pointer) {

	cellsAround[I] = pointer;
	//cellsAroundNext[I] = pointer;

}

int cell::getID() {
	return typeID;
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

	cell* target = cellsAround[int(xf-x) + (int(yf - y) * 3)];
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
	/*
	double averageSurroundTemp = 0;
	for (int i = 0; i < 4; i++) {
		if (&cellsAround[i] != this) {
			averageSurroundTemp += cellsAround[i].getTemp();
		}
	}
	averageSurroundTemp /= 4.0;
	*/
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

	forceComponentVector[0] = 0.0;
	forceComponentVector[1] = 0.0;

}

void cell::calcMagnitudeVectors()
{
	calculateVelocity();

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