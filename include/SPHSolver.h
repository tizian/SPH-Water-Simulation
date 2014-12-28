#pragma once

#include <vector>

#include "Particle.h"
#include "Grid.h"

enum class Visualization
{
	Default,
	Velocity,
	Force,
	Density,
	Pressure,
	Water
};

class SPHSolver
{
public:
	SPHSolver();

	void update(float dt, Visualization vis);
	void render(sf::RenderTarget& rt, Visualization vis);

	void repulsionForce(sf::Vector2f position);
	void attractionForce(sf::Vector2f position);

private:
	int numberParticles;
	std::vector<Particle> particles;
	std::vector<std::vector<int>> neighborhoods;
	Grid grid;

	float kernel(sf::Vector2f x, float h);
	sf::Vector2f gradKernel(sf::Vector2f x, float h);
	float laplaceKernel(sf::Vector2f x, float h);

	void findNeighborhoods();

	void calculateDensity();
	void calculatePressure();

	void calculateForceDensity();
	
	void integrationStep(float dt);

	void collisionHandling();

	void setParticleColors(Visualization vis);

	sf::Texture metaballTexture;
};

