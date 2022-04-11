#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include "grid.h"


class cell {

 private:

	//pointer to parent grid
	grid* parentGrid;

	//seed for random
	unsigned int seed;

	double thermalConductivity;

	//precise x position
	double x;

	//precise y position
	double y;
	//truncated x position
	unsigned int x_t;
	//truncated x position
	unsigned int y_t;

	//thermal variation to avoid pure 0 movement
	//thermal[0] is current x, thermal[1] is current y; 
	//thermal[2] is last x, thermal[3] is last y, used to negate last velocity since thermal should average out
	double* thermal;
	double* thermalInverse;

	//queue of component vectors
	std::queue<double*> forces;

	//component vector that represents the sum of forces
	//formated as [x, y]
	double* forceComponentVector;

	//magnitute and angle vector that represents the sum of forces
	//formated as [magnitute, angle], with the angle being in radians
	double* forceVector;

	//component vector that represents the velocity
	//formated as [x, y]
	double* velocityComponentVector;

	//magnitute and angle vector that represents the cell's current velocity
	//formated as [magnitute, angle], with the angle being in radians
	double* velocityVector;

	//color of cell
	sf::Color color;

 public:

	 //kelvin
	 double temperature;
	 double meltingPoint;

	 //mass is in kg
	 //size is 1 cm cube or 0.01 meters
	 //water has mass 9.97
	 double mass;


	 //basic enumerator
	 // 0 - air
	 // 1 - sand
	 // 2 - water
	 // 3 - snow
	 // 4 - steam
	 // 5 - ice
	 // 6 - molten sand
	 // 10 - solid (wall, used as bounding box)
	 int typeID;

	 //what material the object turns into at melting point
	 int meltIntoID;
	 //what material the object turns into at freezing point
	 int freezeIntoID;

	 cell(unsigned int Seed = 0);
	 cell(grid Grid, unsigned int Seed, int x, int y, sf::Color setColor);

	 /// <summary>
	 /// adds a force component vector to the cell's forceVector queue
	 /// </summary>
	 /// <param name="newVector">: force vector to add to cell </param>
	 /// <returns>bool: completion </returns>
	 bool addForceVector(double* newVector);
	 
	 /// <summary>
	 /// returns the current velocity vector
	 /// </summary>
	 /// <param name="update">: update force vectors and then current velocity vector before returning velocity</param>
	 /// <returns>double[2]: velocity Vector</returns>
	 double* getVelocityVector(bool update=false);

	 sf::Vector2f getPosition();
	 sf::Vector2f getExactPosition();
	 sf::Color getColor();

	 void setColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A = 255);

private:

	void adjustTemperature();

	//generate thermal[0] and thermal[1], seeded random
	void genThermal();

	void sumVectors();

	void calculateVelocity();

	/// <summary>
	/// mag = sqrt(x^2 + y^2)
	/// theta = arctan(y/x)
	/// </summary>
	void calcMagnitudeVectors();

	void calcTrunc();

	//Jon's functions
	void updateMaterial(int id);

};

