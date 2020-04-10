#pragma once


#include "SFML/Graphics.hpp"

class Particle;

class SFMLApp
{


private:

	//Window Settings
	const int antialiasing = 8;
	const int maxFramerate = 60;

	const int heightWindow = 800;
	const int widthWindow = 600;

	int nbBoids = 15;
	float baseSpeed = 100.;

	//Members
	sf::RenderWindow* window_ptr;
	std::vector<Particle*> particles;


	///Methods
	sf::Vector2f getDirectionFromKeyboardInputs();

	void warpParticleIfOutOfBounds(Particle& particle);

public:

	SFMLApp() {};

	int run();

};

