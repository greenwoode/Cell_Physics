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
			return 0.02897;
			break;
		case 1:
			// sand
			return 0.002918475;
			break;
		case 2:
			// water
			return 0.001801528;
			break;
		case 3:
			// snow
			return 0.000090076;
			break;
		case 4:
			// steam
			// Half of water according to Wiki
			return 0.000900764;
			break;
		case 5:
			// ice
			return 0.001621376;
			break;
		case 6:
			// molten sand
			break;
		case 10:
			// solid wall
			return 1.0;
			break;



		default:
			// code block
	}

	return 0.0;

}
