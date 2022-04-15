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

	 void* cellAt(cell* self, double target_x, double target_y);

	 double getMass(cell* self, int id);

	 int getMeltInto(cell* self, int id);

	 int getFreezeInto(cell* self, int id);

	 //Mass
	 //Color
	 //MeltInto
	 //FreezeInto
};

