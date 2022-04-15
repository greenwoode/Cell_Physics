#pragma once
#include "cell.h"

class grid {

 private:
	cell* Grid;

 public:
	 double g;

	 unsigned int width;
	 unsigned int height;

	 bool targetIsEmpty();

	 cell* cellAt(cell* self, double target_x, double target_y);



};

