#include "SPHSolver.h"

#include <iostream>
#include <cmath>

#include "Constants.h"

#ifndef M_PI 
#define M_PI    3.14159265358979323846f 
#endif

using namespace std;
using namespace sf;
using namespace Constants;

SPHSolver::SPHSolver()
{
	int particlesX = NUMBER_PARTICLES / 2.0f;
	int particlesY = NUMBER_PARTICLES;

	numberParticles = particlesX * particlesY;
	particles = vector<Particle>();

	float width = WIDTH / 4.2f;
	float height = 3.0f * HEIGHT / 4.0f;

	FloatRect particleRect = FloatRect((WIDTH - width) / 2, HEIGHT - height, width, height);

	float dx = particleRect.width / particlesX;
	float dy = particleRect.height / particlesY;

	for (int i = 0; i < NUMBER_PARTICLES / 2.0f; i++)
	{
		for (int j = 0; j < NUMBER_PARTICLES; j++)
		{
			Vector2f pos = Vector2f(particleRect.left, particleRect.top) + Vector2f(i * dx, j * dy);
			
			Particle p = Particle(pos);
			particles.push_back(p);
		}
	}

	grid.updateStructure(particles);

	metaballTexture.loadFromFile("res/metaball.png");

	cout << "SPH Solver initialized with " << numberParticles << " particles." << endl;
}

Color getColor(float value, float min, float max)
{
	float lowColor = 0.0f;
	float highColor = 255.0f;

	float color = lowColor + (highColor - lowColor) * (value - min) / (max - min);

	return Color(color, 0, 50, 255);
}

void SPHSolver::setParticleColors(Visualization vis)
{
	float min;
	float max;

	switch (vis)
	{
	case Visualization::Default:

		for (int i = 0; i < numberParticles; i++)
		{
			Color c = Color::White;
			particles[i].renderColor = c;
		}

		break;
	case Visualization::Velocity:
		min = 0.0f;
		max = 20000.0f;

		for (int i = 0; i < numberParticles; i++)
		{
			Color c = getColor(particles[i].getVelocityLength2(), min, max);
			particles[i].renderColor = c;
		}

		break;
	case Visualization::Force:
		min = 10000000000000000.0f;
		max = 2000000000000000000.0f;

		for (int i = 0; i < numberParticles; i++)
		{
			Color c = getColor(particles[i].getForceLength2(), min, max);
			particles[i].renderColor = c;
		}

		break;
	case Visualization::Density:
		min = 2000.0f;
		max = 12000.0f;

		for (int i = 0; i < numberParticles; i++)
		{
			Color c = getColor(particles[i].density, min, max);
			particles[i].renderColor = c;
		}

		break;
	case Visualization::Pressure:
		min = 500.0f;
		max = 30000.0f;

		for (int i = 0; i < numberParticles; i++)
		{
			Color c = getColor(particles[i].pressure, min, max);
			particles[i].renderColor = c;
		}

		break;
	default:
		break;
	}
}

void SPHSolver::render(RenderTarget &rt, Visualization vis)
{
	if (vis != Visualization::Water)
	{
		CircleShape circle(0.5f * PARTICLE_SPACING * SCALE);

		for (int i = 0; i < numberParticles; i++)
		{
			//Color c = Color(2 * particles[i].normalLength, 0, 40, 255);
			circle.setFillColor(particles[i].renderColor);

			circle.setPosition(particles[i].position * SCALE);
			rt.draw(circle);
		}
	}
	else
	{
		Sprite sprite;
		sprite.setTexture(metaballTexture);
		sprite.setOrigin(metaballTexture.getSize().x / 2.0f, metaballTexture.getSize().y / 2.0f);
		float scale = 4.2f * PARTICLE_SPACING * SCALE / 256.0f;
		sprite.setScale(scale, scale);

		for (int i = 0; i < numberParticles; i++)
		{
			//sprite.setScale( Vector2f(0.18f, 0.18f) * (particles[i].position.y));
			sprite.setPosition(particles[i].position * SCALE);
			rt.draw(sprite, BlendAdd);
		}
	}
}

void SPHSolver::repulsionForce(Vector2f position)
{
	for (int i = 0; i < numberParticles; i++)
	{
		Vector2f x = particles[i].position - position;

		float dist2 = x.x * x.x + x.y * x.y;

		if (dist2 < KERNEL_RANGE * 3)
		{
			particles[i].force += x * 800000.0f * particles[i].density;
		}
	}
}

void SPHSolver::attractionForce(Vector2f position)
{
	for (int i = 0; i < numberParticles; i++)
	{
		Vector2f x = position - particles[i].position;

		float dist2 = x.x * x.x + x.y * x.y;

		if (dist2 < KERNEL_RANGE * 3)
		{
			particles[i].force += x * 800000.0f * particles[i].density;
		}
	}
}

void SPHSolver::update(float dt, Visualization vis)
{
	findNeighborhoods();
	calculateDensity();
	calculatePressure();
	calculateForceDensity();
	integrationStep(dt);
	collisionHandling();

	if (vis != Visualization::Water)
	{
		setParticleColors(vis);
	}
	

	for (int i = 0; i < numberParticles; i++)
	{
		particles[i].force = Vector2f(0.0f, 0.0f);
	}

	grid.updateStructure(particles);
}

// Poly6 Kernel
float SPHSolver::kernel(sf::Vector2f x, float h)
{
	float r2 = x.x * x.x + x.y * x.y;
	float h2 = h * h;

	if (r2 < 0 || r2 > h2) return 0.0f;

	return 315.0f / (64.0f * M_PI * pow(h, 9)) * pow(h2 - r2, 3);
}

// Gradient of Spiky Kernel
Vector2f SPHSolver::gradKernel(sf::Vector2f x, float h)
{
	float r = sqrt(x.x * x.x + x.y * x.y);
	if (r == 0.0f) return Vector2f(0.0f, 0.0f);

	float t1 = -45.0f / (M_PI * pow(h, 6));
	Vector2f t2 = x / r;
	float t3 = pow(h - r, 2);


	return  t1 * t2 * t3;
}

// Laplacian of Viscosity Kernel
float SPHSolver::laplaceKernel(sf::Vector2f x, float h)
{
	float r = sqrt(x.x * x.x + x.y * x.y);
	return 45.0f / (M_PI * pow(h, 6)) * (h - r);
}

void SPHSolver::findNeighborhoods()
{
	neighborhoods = vector<vector<int>>();
	float maxDist2 = KERNEL_RANGE * KERNEL_RANGE;

	for each (const Particle &p in particles)
	{
		vector<int> neighbors = vector<int>();
		vector<Cell> neighboringCells = grid.getNeighboringCells(p.position);

		for each (const Cell &cell in neighboringCells)
		{
			for each (int index in cell)
			{
				Vector2f x = p.position - particles[index].position;
				float dist2 = x.x * x.x + x.y * x.y;
				if (dist2 <= maxDist2) {
					neighbors.push_back(index);
				}
			}
		}

		neighborhoods.push_back(neighbors);
	}
}

void SPHSolver::calculateDensity()
{
	for (int i = 0; i < numberParticles; i++)
	{
		vector<int> neighbors = neighborhoods[i];
		float densitySum = 0.0f;

		for (int n = 0; n < neighbors.size(); n++)
		{
			int j = neighbors[n];

			Vector2f x = particles[i].position - particles[j].position;
			densitySum += particles[j].mass * kernel(x, KERNEL_RANGE);
		}

		particles[i].density = densitySum;
	}
}

void SPHSolver::calculatePressure()
{
	for (int i = 0; i < numberParticles; i++)
	{
		particles[i].pressure = max(STIFFNESS * (particles[i].density - REST_DENSITY), 0.0f);
	}
}

void SPHSolver::calculateForceDensity()
{
	for (int i = 0; i < numberParticles; i++)
	{
		Vector2f fPressure = Vector2f(0.0f, 0.0f);
		Vector2f fViscosity = Vector2f(0.0f, 0.0f);
		Vector2f fGravity = Vector2f(0.0f, 0.0f);

		vector<int> neighbors = neighborhoods[i];
		
		//particles[i].color = 0;

		for (int n = 0; n < neighbors.size(); n++)
		{
			int j = neighbors[n];
			Vector2f x = particles[i].position - particles[j].position;

			// Pressure force density
			fPressure += particles[j].mass * (particles[i].pressure + particles[j].pressure) / (2.0f * particles[j].density) * gradKernel(x, KERNEL_RANGE);

			// Viscosity force density
			fViscosity += particles[j].mass * (particles[j].velocity - particles[i].velocity) / particles[j].density * laplaceKernel(x, KERNEL_RANGE);

			// Color field
			//particles[i].color += particles[j].mass / particles[j].density * kernel(x, KERNEL_RANGE);
		}

		//particles[i].normal = Vector2f(0.0f, 0.0f);

		//for (int n = 0; n < neighbors.size(); n++)
		//{
		//	int j = neighbors[n];
		//	Vector2f x = particles[i].position - particles[j].position;

		//	////particles[i].normal += particles[j].mass / particles[j].density * gradKernel(x, KERNEL_RANGE);
		//}

		//float normalLength = sqrt(particles[i].normal.x * particles[i].normal.x + particles[i].normal.y * particles[i].normal.y);

		//Vector2f fSurface = Vector2f(0.0f, 0.0f);

		//for (int n = 0; n < neighbors.size(); n++)
		//{
		//	int j = neighbors[n];
		//	Vector2f x = particles[i].position - particles[j].position;

		//	if (normalLength > 0.1f)
		//	{
		//		fSurface += -TENSION * (particles[j].mass / particles[j].density * laplaceKernel(x, KERNEL_RANGE)) * particles[i].normal / normalLength;
		//	}
		//}

		// Gravitational force density
		fGravity = particles[i].density * Vector2f(0, GRAVITY);

		fPressure *= -1.0f;
		fViscosity *= VISCOCITY;

		//particles[i].force += fPressure + fViscosity + fGravity + fSurface;
		particles[i].force += fPressure + fViscosity + fGravity;
	}
}

void SPHSolver::integrationStep(float dt)
{
	for (int i = 0; i < numberParticles; i++)
	{
		particles[i].velocity += dt * particles[i].force / particles[i].density;
		particles[i].position += dt * particles[i].velocity;
	}
}

void SPHSolver::collisionHandling()
{
	for (int i = 0; i < numberParticles; i++)
	{
		if (particles[i].position.x < 0.0f)
		{
			particles[i].position.x = 0.0f;
			particles[i].velocity.x = -0.5f * particles[i].velocity.x;
		}
		else if (particles[i].position.x > WIDTH)
		{
			particles[i].position.x = WIDTH;
			particles[i].velocity.x = -0.5f * particles[i].velocity.x;
		}
		
		if (particles[i].position.y < 0.0f)
		{
			particles[i].position.y = 0.0f;
			particles[i].velocity.y = -0.5f * particles[i].velocity.y;
		}
		else if (particles[i].position.y > HEIGHT)
		{
			particles[i].position.y = HEIGHT;
			particles[i].velocity.y = -0.5f * particles[i].velocity.y;
		}
	}
}
