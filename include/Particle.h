#pragma once

#include <SFML/Graphics.hpp>

class Particle
{
public:
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f force;

	float mass;
	float density;
	float pressure;

	float color;
	sf::Vector2f normal;

	Particle();
	Particle(sf::Vector2f position);

	sf::Color renderColor;

	float getVelocityLength2() const;
	float getForceLength2() const;
	float getNormalLength2() const;
};