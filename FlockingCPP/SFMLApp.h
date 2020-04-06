#pragma once


#include "SFML/Graphics.hpp"

#include "Boid.h"

class SFMLApp
{


private:

	//Window Settings
	const int antialiasing = 8;
	const int maxFramerate = 60;

	const int heightWindow = 800;
	const int widthWindow = 600;

	//Members
	std::vector<Boid> boids;

	///Methods
	sf::Vector2f getDirectionFromKeyboardInputs();

public:

	SFMLApp() {};

	int run();

};

