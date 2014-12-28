#include "Grid.h"

#include <iostream>
#include <cmath>

#include "Constants.h"

using namespace std;

Grid::Grid()
{
	numberCellsX = Constants::WIDTH / Constants::KERNEL_RANGE + 1;
	numberCellsY = Constants::HEIGHT / Constants::KERNEL_RANGE + 1;

	cout << "Grid with " << numberCellsX << " x " << numberCellsY << " cells created." << endl;
}

vector<Cell> Grid::getNeighboringCells(sf::Vector2f position)
{
	vector<Cell> resultCells = vector<Cell>();

	int xCell = position.x / Constants::KERNEL_RANGE;
	int yCell = position.y / Constants::KERNEL_RANGE;

	resultCells.push_back(cells[xCell][yCell]);
	if (xCell > 0) resultCells.push_back(cells[xCell - 1][yCell]);
	if (xCell < numberCellsX - 1) resultCells.push_back(cells[xCell + 1][yCell]);

	if (yCell > 0) resultCells.push_back(cells[xCell][yCell - 1]);
	if (yCell < numberCellsY - 1) resultCells.push_back(cells[xCell][yCell + 1]);

	if (xCell > 0 && yCell > 0) resultCells.push_back(cells[xCell - 1][yCell - 1]);
	if (xCell > 0 && yCell < numberCellsY - 1) resultCells.push_back(cells[xCell - 1][yCell + 1]);
	if (xCell < numberCellsX - 1 && yCell > 0) resultCells.push_back(cells[xCell + 1][yCell - 1]);
	if (xCell < numberCellsX - 1 && yCell < numberCellsY - 1) resultCells.push_back(cells[xCell + 1][yCell + 1]);

	return resultCells;
}

void Grid::updateStructure(std::vector<Particle> &particles)
{
	cells = vector<vector<Cell>>(numberCellsX, vector<Cell>(numberCellsY, Cell()));

	for (int i = 0; i < particles.size(); i++)
	{
		int xCell = particles[i].position.x / Constants::KERNEL_RANGE;
		int yCell = particles[i].position.y / Constants::KERNEL_RANGE;

		cells[xCell][yCell].push_back(i);
	}
}