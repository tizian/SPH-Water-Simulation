#pragma once

namespace Constants
{
	const float WIDTH = 3;
	const float HEIGHT = 1.5;

	const float SCALE = 400;	// pixels per meter

	const int RENDER_WIDTH = SCALE * WIDTH;
	const int RENDER_HEIGHT = SCALE * HEIGHT;

	const float WINDOW_SCALE = 1.0f;

	const float TIMESTEP = 0.0001f;

	const int NUMBER_PARTICLES = 70;	// Along one dimension
	
	const float REST_DENSITY = 1000;

	const float STIFFNESS = 10000;
	const float VISCOCITY = 12000;
	const float TENSION = 10000.0f;

	const float GRAVITY = 12000;

	const float PARTICLE_SPACING = 1.0f / NUMBER_PARTICLES;
	const float PARTICLE_VOLUME = PARTICLE_SPACING * PARTICLE_SPACING;
	const float PARTICLE_MASS = PARTICLE_VOLUME * REST_DENSITY;
	const float KERNEL_RANGE = 2 * PARTICLE_SPACING;
}