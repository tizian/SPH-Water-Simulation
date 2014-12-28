#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

#include "Constants.h"
#include "SPHSolver.h"

using namespace std;
using namespace sf;
using namespace Constants;

const string APPLICATION_NAME = "Smooth Particle Hydrodynamics";

Visualization vis = Visualization::Default;

Shader shader;

SPHSolver sph = SPHSolver();

bool pause = false;

void init();
void update(float);
void render(RenderWindow &, RenderTexture &);
void toggleVisualization();

int main()
{
	RenderWindow window(VideoMode(RENDER_WIDTH * WINDOW_SCALE, RENDER_HEIGHT * WINDOW_SCALE), APPLICATION_NAME);
	window.setKeyRepeatEnabled(false);

	RenderTexture renderTexture;
	renderTexture.create(RENDER_WIDTH, RENDER_HEIGHT);

	init();

	float time = 0.0f;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape)
				{
					window.close();
				}

				if (event.key.code == Keyboard::Tab)
				{
					toggleVisualization();
				}

				if (event.key.code == Keyboard::Space)
				{
					pause = !pause;
				}

				if (event.key.code == Keyboard::Num1)
				{
					sph = SPHSolver();
					vis = Visualization::Default;
				}

				if (event.key.code == Keyboard::Num2)
				{
					sph = SPHSolver();
					vis = Visualization::Velocity;
				}

				if (event.key.code == Keyboard::Num3)
				{
					sph = SPHSolver();
					vis = Visualization::Force;
				}

				if (event.key.code == Keyboard::Num4)
				{
					sph = SPHSolver();
					vis = Visualization::Density;
				}

				if (event.key.code == Keyboard::Num5)
				{
					sph = SPHSolver();
					vis = Visualization::Pressure;
				}

				if (event.key.code == Keyboard::Num6)
				{
					sph = SPHSolver();
					vis = Visualization::Water;
				}
			}

			if (event.type == Event::MouseButtonPressed)
			{
				float x = event.mouseButton.x / WINDOW_SCALE / SCALE;
				float y = event.mouseButton.y / WINDOW_SCALE / SCALE;
				Vector2f position = Vector2f(x, y);

				if (event.mouseButton.button == Mouse::Left)
				{
					sph.repulsionForce(position);
				}

				if (event.mouseButton.button == Mouse::Right)
				{
					sph.attractionForce(position);
				}
			}
		}

		switch (vis)
		{
		case Visualization::Default:
			window.setTitle(APPLICATION_NAME + " - Visualization: Particles");
			break;
		case Visualization::Velocity:
			window.setTitle(APPLICATION_NAME + " - Visualization: Particle velocity");
			break;
		case Visualization::Force:
			window.setTitle(APPLICATION_NAME + " - Visualization: Particle force");
			break;
		case Visualization::Density:
			window.setTitle(APPLICATION_NAME + " - Visualization: Particle density");
			break;
		case Visualization::Pressure:
			window.setTitle(APPLICATION_NAME + " - Visualization: Particle pressure");
			break;
		case Visualization::Water:
			window.setTitle(APPLICATION_NAME + " - Visualization: Water");
			break;
		default:
			break;
		}

		if (!pause)
		{
			update(TIMESTEP);
			time += TIMESTEP;
		}
		
		//cout << time << endl;

		renderTexture.clear(Color::Black);

		render(window, renderTexture);
		
		window.display();
	}

	return 0;
}

void init()
{
	shader.loadFromFile("res/shader.vert", "res/shader.frag");
}

void update(float dt)
{
	sph.update(dt, vis);
}

void render(RenderWindow &window, RenderTexture &renderTexture)
{
	sph.render(renderTexture, vis);
	renderTexture.display();
	renderTexture.setSmooth(true);

	Sprite sprite = Sprite(renderTexture.getTexture());
	sprite.setScale(WINDOW_SCALE, WINDOW_SCALE);

	if (vis != Visualization::Water)
	{
		window.draw(sprite);
	}
	else
	{
		shader.setParameter("texture", Shader::CurrentTexture);
		window.draw(sprite, &shader);
	}
}

void toggleVisualization()
{
	switch (vis)
	{
	case Visualization::Default:
		vis = Visualization::Velocity;
		break;
	case Visualization::Velocity:
		vis = Visualization::Force;
		break;
	case Visualization::Force:
		vis = Visualization::Density;
		break;
	case Visualization::Density:
		vis = Visualization::Pressure;
		break;
	case Visualization::Pressure:
		vis = Visualization::Water;
		break;
	case Visualization::Water:
		vis = Visualization::Default;
		break;
	default:
		break;
	}
}