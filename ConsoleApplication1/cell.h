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

	double timeScale;

	double thermalConductivity;

	// old x position
	double oldX;
	// old y position
	double oldY;

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
	//water has mass 0.001
	double mass;

	 //kelvin
	 double temperature;
	 double meltingPoint;

	 double otherMass;
	 double* otherInitialVComp;
	 double* ourInitalVComp;
	 double totalMass;

	 double* FinalVelocity;
	 double* otherFinalVelocity;

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

	 double getMass();

	 /// <summer>
	 /// get the direction the cell is moving in the X axis
	 /// </summer>
	 int getDirX();

	 /// <summer>
	 /// get the direction the cell is moving in the Y axis
	 /// </summer>
	 int getDirY();

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

	 /// <summary>
	 /// This function move 
	 /// </summary>
	 /// <param name="timeStep">: This will scale cell movement base on largest velocity (pass in from grid)</param>
	 void update(double timeStep);

	 /// <summary>
	 /// move cell according to velocity vector
	 /// </summary>
	 /// <param name="timeScale">: This will scale cell movement base on largest velocity (pass in from grid)</param>
	 void move(double timeScale);

	 /// <summary>
	 /// move cell according to velocity vector
	 /// However this one will impact with another cell first then move
	 /// </summary>
	 /// <param name="otherCell">: The cell which our cell is colliding with</param>
	 /// <param name="timeScale">: This will scale cell movement base on largest velocity (pass in from grid)</param>
	 void move(cell *otherCell, double timeScale);

	 /// <summary>
	 /// move cell according to velocity vector
	 /// However this one will impact with 2 cell first then move
	 /// </summary>
	 /// <param name="otherCellX">: The cell which our cell is colliding with on the X axis</param>
	 /// <param name="otherCellY">: The cell which our cell is colliding with on the Y axis</param>
	 /// <param name="timeScale">: This will scale cell movement base on largest velocity (pass in from grid)</param>
	 void move(cell *otherCellX, cell *otherCellY, double timeScale);

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
	 void impact(cell *otherCell);

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
	//TEMP USED FOR STORAGE
	void updateInformation();

};

