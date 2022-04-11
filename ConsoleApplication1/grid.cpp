#include "grid.h"


bool grid::targetIsEmpty()
{
	return false;
}

cell* grid::cellAt(cell* self, double target_x, double target_y)
{
	if(!targetIsEmpty())
		return &Grid[int(trunc(target_x) + (trunc(target_y)* width))];

	return self;
}
