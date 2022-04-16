#include "grid.h"
#include <iostream>

grid::grid(int Width, int Height, double gravity) {
	width = Width;
	height = Height;
	g = new double[2];
	g[0] = 0.0;
	g[1] = gravity;

	Grid = new cell*[Width*Height];
	printf("created Grid\n");
	GridFuture = new cell*[Width * Height];
	printf("created GridFuture\n");
	//clearGrid();
	//flush();
	initialFlush();
	printf("cleared Grid\n");
}

void grid::clearGrid() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			deleteFutureCell( i, j );
		}
		printf("deleted row %d\n", i);
	}
}

int grid::getColorRAt(int target_x, int target_y) {

	if (!targetIsEmpty(target_x, target_y))
		return (*Grid[target_x + (target_y * width)]).getColorR();

	return 0;
}

int grid::getColorGAt(int target_x, int target_y) {

	if (!targetIsEmpty(target_x, target_y))
		return (*Grid[target_x + (target_y * width)]).getColorG();

	return 0;
}

int grid::getColorBAt(int target_x, int target_y) {

	if (!targetIsEmpty(target_x, target_y))
		return (*Grid[target_x + (target_y * width)]).getColorB();

	return 0;
}

void grid::initialFlush() {

	memset(Grid, 0, width*height*sizeof(nullptr));
	memset(GridFuture, 0, width * height * sizeof(nullptr));

}

bool grid::targetIsEmpty(int target_x, int target_y)
{
	return Grid[target_x + (target_y * width)] == nullptr;
}

bool grid::addCell(unsigned int Seed, int X, int Y, int ColorR, int ColorG, int ColorB, int ID, double Mass) {
	if (targetIsEmpty(X, Y)) {
		Grid[X + (Y * width)] = new cell(Seed, X, Y, ColorR, ColorG, ColorB, ID, Mass);
		return true;
	}
	return false;
}

void grid::removeCell(int target_x, int target_y) {

	if (!targetIsEmpty(target_x, target_y))
		memset(Grid[target_x + (target_y * width)], 0, sizeof(nullptr));

}

void grid::deleteCell(int target_x, int target_y) {

	if (!targetIsEmpty(target_x, target_y)) {
		delete Grid[target_x + (target_y * width)];
		memset(Grid[target_x + (target_y * width)], 0, sizeof(nullptr));
	}

}

void grid::deleteFutureCell(int target_x, int target_y) {

	printf("deleting cell...  ");
	if (!targetIsEmpty(target_x, target_y)) {
		delete Grid[target_x + (target_y * width)];
		printf("deleted\n");
		Grid[target_x + (target_y * width)] = nullptr;
	}
	printf("nullptr assigned\n");

}

void grid::flush() {
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			Grid[i + (j * width)] = GridFuture[i + (j * width)];
			GridFuture[i + (j * width)] = nullptr;
		}
	}
}

double grid::calcTimeStep()
{
	double maxSpeed = 0;
	double temp;
	for (int i = 0; i < width * height; i++) {
		if ((*Grid[i]).getID()) {
			temp = (*Grid[i]).getVelocityVector()[0];
			if (temp > maxSpeed)
				maxSpeed = temp;
		}
		
	}
	return std::max(1.0, 1.0 / maxSpeed);
}

void grid::update()
{
	timeStep = calcTimeStep();
	flush();
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			if (Grid[i + (j * width)]) {
				sf::Vector2f pos = (*Grid[i + (j * width)]).getPosition();
				for (int q = 0; q < 3; q++) {
					for (int k = 0; k < 3; k++)
						(*Grid[i + (j * width)]).setNeighbor(q + k * 3, Grid[((int(pos.x) - 1) + q) + ((int(pos.y) - 1) + k) * width]);
				}
				(*Grid[i]).update(timeStep, g);
			}
		}
	}
	for (int i = 0; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			if (Grid[i + (j * width)]) {
				sf::Vector2f pos = (*Grid[i + (j * width)]).getFuturePosition();
				GridFuture[int(pos.x) + int(pos.y) * width] = Grid[i + (j * width)];
			}
		}
	}
	
}

cell* grid::cellAt(cell* self, int target_x, int target_y)
{
	if(!targetIsEmpty(target_x, target_y))
		return Grid[int(trunc(target_x) + (trunc(target_y)* width))];

	return self;
}
double grid::getTimestep()
{
	return timeStep;
}
/*
double grid::getMass(cell * self, int id) {
	switch (id) {
		case 0:
			// solid wall
			return 1000000;
			break;
		case 1:
			// air
			return 0.00000129;
			break;
		case 2:
			// sand
			return 0.00153;
			break;
		case 3:
			// water
			return 0.0009998;
			break;
		case 4:
			// snow
			return 0.000090076;
			break;
		case 5:
			// steam
			// Half of water according to Wiki
			return 0.000900764;
			break;
		case 6:
			// ice
			return 0.001621376;
			break;
		case 7:
			// molten sand
			//TODO: Define molar mass
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