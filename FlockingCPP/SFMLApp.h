#pragma once


#include "SFML/Graphics.hpp"

#include "Particle.h"

class SFMLApp
{


private:

	//Window Settings
	const int antialiasing = 8;
	const int maxFramerate = 60;

	const int heightWindow = 800;
	const int widthWindow = 600;

	//Members
	sf::RenderWindow* window_ptr;
	std::vector<Particle> boids;

	///Methods
	sf::Vector2f getDirectionFromKeyboardInputs();

	void warpParticleIfOutOfBounds(Particle& particle);

public:

	SFMLApp() {};

	int run();

};

