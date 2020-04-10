#pragma once


#include "SFML/Graphics.hpp"
#include "Boid.h"

class FlockingRule
{

private:

	float weight;
	sf::Vector2f force;


	//if displayed
	sf::Color debugColor;

public:


	virtual sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood) = 0;

};

