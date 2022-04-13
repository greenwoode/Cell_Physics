#include "grid.h"

grid::grid(int Width, int Height, double gravity) {
	width = Width;
	height = Height;
	g = gravity;

	Grid = new cell*[Width*Height];
	clearGrid();
}

void grid::clearGrid() {

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

cell* grid::cellAt(cell* self, int target_x, int target_y)
{
	if(!targetIsEmpty(target_x, target_y))
		return Grid[int(trunc(target_x) + (trunc(target_y)* width))];

	return self;
}
