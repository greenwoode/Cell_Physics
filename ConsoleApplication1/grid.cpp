#include "grid.h"

grid::grid(int Width, int Height, double gravity) {
	width = Width;
	height = Height;
	g = gravity;

	Grid = new cell*[Width*Height];
	GridFuture = new cell*[Width * Height];;
	clearGrid();
}

void grid::clearGrid() {

	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			deleteCell( i, j );
		}
	}

}

sf::Color grid::getColorAt(int target_x, int target_y) {

	if (!targetIsEmpty(target_x, target_y))
		return (*Grid[target_x + (target_y * width)]).getColor();

	return sf::Color();
}

bool grid::targetIsEmpty(int target_x, int target_y)
{
	return &Grid[target_x + (target_y * width)] == nullptr;
}

bool grid::addCell(cell* Cell, int target_x, int target_y) {
	if (targetIsEmpty(target_x, target_y)) {
		Grid[target_x + (target_y * width)] = Cell;
		return true;
	}
	return false;
}

void grid::removeCell(int target_x, int target_y) {

	if (!targetIsEmpty(target_x, target_y))
		Grid[target_x + (target_y * width)] = nullptr;

}

void grid::deleteCell(int target_x, int target_y) {

	if (!targetIsEmpty(target_x, target_y)) {
		delete Grid[target_x + (target_y * width)];
		Grid[target_x + (target_y * width)] = nullptr;
	}

}

void grid::flush() {
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			Grid[i + (j * width)] = GridFuture[i + (j * width)];
		}
	}
}

double grid::calcTimeStep()
{
	double maxSpeed = 0;
	double temp;
	for (int i = 0; i < width * height; i++) {
		temp = (*Grid[i]).getVelocityVector()[0];
		if (temp > maxSpeed)
			maxSpeed = temp;
	}
	return std::max(1.0, 1.0/maxSpeed);
}

void grid::update()
{
	timeStep = calcTimeStep();
	for (int i = 0; i < width * height; i++) {
		if (!Grid[i]) {
			(*Grid[i]).update(timeStep);
		}
	}
	//sf::Vector2f pos = (*Grid[i]).getFuturePosition();
	//GridFuture[int(pos.x) + int(pos.y)*width] = Grid[i];
}

cell* grid::cellAt(cell* self, int target_x, int target_y)
{
	if(!targetIsEmpty(target_x, target_y))
		return Grid[int(trunc(target_x) + (trunc(target_y)* width))];

	return self;
}
/*
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
*/