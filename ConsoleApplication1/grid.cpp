#include "grid.h"

bool grid::targetIsEmpty()
{
	return false;
}

void* grid::cellAt(cell* self, double target_x, double target_y)
{
	if(!targetIsEmpty())
		return &Grid[int(trunc(target_x) + (trunc(target_y)* width))];

	return self;
}

double grid::getMass(cell * self, int id) {
	switch (id) {
		case 0:
			// air
			break;
		case 1:
			// sand
			break;
		case 2:
			// water
			break;
		case 3:
			// snow
			break;
		case 4:
			// steam
			break;
		case 5:
			// ice
			break;
		case 6:
			// molten sand
			break;
		case 10:
			// solid wall
			break;



		default:
			// code block
	}

	return 0.0;

}
