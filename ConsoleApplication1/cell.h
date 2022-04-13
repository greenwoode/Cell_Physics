#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
//#include "grid.h"


class cell {

 private:

	 
	 cell* cellsAround;
	 cell* cellsAroundNext;

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

	//mass is in kg
	//size is 1 cm cube or 0.01 meters
	//water has mass 9.97
	double mass;

	//kelvin
	double temperature;

 public:

	 cell(unsigned int Seed = 0);
	 cell(unsigned int Seed, int x, int y, sf::Color setColor);

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

	 /// <summary>
	 /// Return the velocity componant vector of this cell
	 /// </summary>
	 /// <returns>double[2]: velocity/force component vector</return>
	 double* getVelocityComponentVector();


	 sf::Vector2f getPosition();
	 sf::Vector2f getExactPosition();
	 sf::Color getColor();
	 double getTemp();

	 ///uses pointers
	 ///up = [0]
	 ///down = [1]
	 ///left = [2]
	 ///right = [3]
	 void setNeighbor(int Dir, cell* pointer);

	 void setColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A = 255);

	 /// Just move stuff
	 int move(int addX, int addY);

	 int move(cell otherCell);


	 /// <summary>
	 /// This method check if our cell is collisding with anothercell
	 /// It start by checking if the velocity vector is 0 or not
	 /// If yes then check the neighbor in its relative position
	 /// </summary>
	 /// <param name="otherCell">: The cell which our cell is colliding with</param>
	 /// <returns>return true and call the impact function when it does colide with something</returns>
	 bool collisionCheck(cell otherCell);

	 /// <summary>
	 /// Calculate the force that is excerted onto both cell after they impacted
	 /// using 2 equations:
	 ///	U1 = ((m1 - m2)(u1 - u2))/(m1 + m2) + u2
	 ///	U2 = (2m1(u1 - u2)) / (m1 + m2) + u2
	 ///	U: Final Velocity
	 ///	u: Inital Velocity
	 ///	m: mass
	 ///	1: our Cell
	 ///	2: other cell
	 /// </summary>
	 /// <param name="otherCell">: The cell which our cell is colliding with</param>
	 /// <returns>Does not return anything but add a force vector to our cell and otherCell</returns>
	 void impact(cell otherCell);

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

};

