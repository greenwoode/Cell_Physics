#pragma once
#include "cell.h"
#include <SFML/Graphics.hpp>

class grid {

 private:
	cell** Grid;
	cell** GridFuture;

	//[0.0, g] with negative g tending toward bottom of board
	double* g;
	double timeStep;

	unsigned int width;
	unsigned int height;

	void clearGrid();

 public:

	 grid(int Width, int Height, double gravity);

	 bool addCell(cell* Cell, int target_x, int target_y);

	 void removeCell(int target_x, int target_y);

	 void deleteCell(int target_x, int target_y);

	 void deleteFutureCell(int target_x, int target_y);

	 void flush();

	 double calcTimeStep();

	 void update();
	 int getColorRAt(int target_x, int target_y);
	 int getColorGAt(int target_x, int target_y);
	 int getColorBAt(int target_x, int target_y);

	 bool targetIsEmpty(int target_x, int target_y);

	 cell* cellAt(cell* self, int target_x, int target_y);

	 double getTimestep();

	 double getMass(cell* self, int id);

	 int getMeltInto(cell* self, int id);

	 int getFreezeInto(cell* self, int id);

	 //Mass
	 //Color
	 //MeltInto
	 //FreezeInto
};

