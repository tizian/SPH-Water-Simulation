#pragma once

#include <vector>
#include "Particle.h"

typedef std::vector<int> Cell;

class Grid
{
public:
	Grid();

	void updateStructure(std::vector<Particle> &particles);

	std::vector<Cell> getNeighboringCells(sf::Vector2f position);
	
private:
	int numberCellsX;
	int numberCellsY;

	std::vector<std::vector<Cell>> cells;
};

