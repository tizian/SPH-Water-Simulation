#include "Particle.h"

#include "Constants.h"

using namespace sf;

Particle::Particle()
{
	Particle(Vector2f());
}

Particle::Particle(Vector2f pos)
{
	position = pos;
	velocity = Vector2f();
	force = Vector2f();

	mass = Constants::PARTICLE_MASS;

	density = 0;
	pressure = 0;

	color = 0;
	normal = Vector2f();
}

float Particle::getVelocityLength2() const
{
	return velocity.x * velocity.x + velocity.y * velocity.y;
}

float Particle::getForceLength2() const
{
	return force.x * force.x + force.y * force.y;
}

float Particle::getNormalLength2() const
{
	return normal.x * normal.x + normal.y * normal.y;
}
