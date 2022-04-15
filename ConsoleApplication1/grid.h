#pragma once
#include "cell.h"
#include <SFML/Graphics.hpp>

class grid {

 private:
	cell** Grid;
	cell** GridFuture;

	double g;
	double timeStep;

	unsigned int width;
	unsigned int height;

	void clearGrid();

 public:

	 grid(int Width, int Height, double gravity);

	 bool addCell(cell* Cell, int target_x, int target_y);

	 void removeCell(int target_x, int target_y);

	 void deleteCell(int target_x, int target_y);

	 void flush();

	 double calcTimestep();

	 void update();

	 sf::Color getColorAt(int target_x, int target_y);

	 bool targetIsEmpty(int target_x, int target_y);

	 cell* cellAt(cell* self, int target_x, int target_y);

};

