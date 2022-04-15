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
			//TODO: Define molar mass
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

int grid::getMeltInto(cell* self, int id) {
	switch (id) {
	case 0:
		// air
		return 0;
		break;
	case 1:
		// sand
		return 6;
		break;
	case 2:
		// water
		return 4;
		break;
	case 3:
		// snow
		return 2;
		break;
	case 4:
		// steam
		return 4;
		break;
	case 5:
		// ice
		return 2;
		break;
	case 6:
		// molten sand
		break;
	case 10:
		// solid wall
		return 10;
		break;



	default:
		return id;
	}

}

int grid::getFreezeInto(cell* self, int id) {
	switch (id) {
	case 0:
		// air
		return 0;
		break;
	case 1:
		// sand
		return 1;
		break;
	case 2:
		// water
		return 5;
		break;
	case 3:
		// snow
		return 5;
		break;
	case 4:
		// steam
		return 2;
		break;
	case 5:
		// ice
		return 5;
		break;
	case 6:
		// molten sand
		//TODO
		break;
	case 10:
		// solid wall
		return 10;
		break;



	default:
		return id;
	}

}
