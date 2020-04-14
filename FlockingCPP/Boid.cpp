#include "Boid.h"

#include "FlockingRule.h"

#include <iostream>

using namespace utils;

std::vector<Boid*> Boid::computeBoidNeighbordhood()
{
	std::vector<Boid*> neighborhood;

	for (const auto& p : *particles)
	{
		Boid* boid = dynamic_cast<Boid*>(p);

		if (boid != nullptr && boid != this) {

			float distance = vector2::getDistance(getPosition(), boid->getPosition());
			//Verify if boid is close enough to be part of the neighborhood
			if (distance <= detectionRadius)
			{
				neighborhood.push_back(boid);
			}
		}
		
	}

	return neighborhood;
}


Boid::Boid(std::vector<Particle*>* particles_) : Particle(), particles(particles_)
{

}

std::vector<sf::Drawable*> Boid::toDraw()
{
	std::vector<sf::Drawable*> shapesToDraw = Particle::toDraw();

	if (drawDebugRadius) {

		//Display radius detection
		sf::CircleShape* vision = new sf::CircleShape(detectionRadius);

		vision->setFillColor(sf::Color::Transparent);
		vision->setOutlineThickness(1.);
		vision->setOrigin(vision->getRadius(), vision->getRadius());
		vision->setOutlineColor(sf::Color::Blue);
		vision->setPosition(getPosition());

		shapesToDraw.push_back(vision);

	}

	if (drawDebugRules)
	{
		//Display rules
		for (auto& rule : rules)
		{
			shapesToDraw.push_back(rule->getVectorShape(this));
		}
	}

	return shapesToDraw;
}

void Boid::update(const float deltaTime)
{
	Particle::update(deltaTime);

	std::vector<Boid*> neighbordhood = computeBoidNeighbordhood();

	for (auto& rule : rules) 
	{
		sf::Vector2f weightedForce = rule->computeWeightedForce(neighbordhood, this);
		//std::cout << typeid(*rule).name() << " Force : " << vector2::getMagnitude(weightedForce) << std::endl;
		applyForce(weightedForce);
	}

}
