#include "Boid.h"

#include "FlockingRule.h"

#include <iostream>

using namespace Utils;

std::vector<Boid*> Boid::computeBoidNeighbordhood()
{
	std::cout << "Size : " << particles->size() << std::endl;
	std::vector<Boid*> neighborhood;

	//std::cout << "Size : " << neighborhood.size() << std::endl;

	for (const auto& p : *particles)
	{
		Boid* boid = dynamic_cast<Boid*>(p);

		if (boid != nullptr && boid != this) {

			float distance = Vector2::getDistance(getPosition(), boid->getPosition());
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
	rules.push_back(new SeparationRule(600));
	rules.push_back(new CohesionRule(0.2));
	rules.push_back(new AlignmentRule(0.02));
}

std::vector<sf::Drawable*> Boid::toDraw()
{
	std::vector<sf::Drawable*> shapesToDraw = Particle::toDraw();

	if (drawDebug) {

		//Display radius detection
		sf::CircleShape* vision = new sf::CircleShape(detectionRadius);

		vision->setFillColor(sf::Color::Transparent);
		vision->setOutlineThickness(1.);
		vision->setOrigin(vision->getRadius(), vision->getRadius());
		vision->setOutlineColor(sf::Color::Blue);
		vision->setPosition(getPosition());

		shapesToDraw.push_back(vision);

		//Display rules
		for (auto& rule : rules)
		{
			shapesToDraw.push_back(rule->getVectorShape(this));
		}

		//Display velocity
		//sf::Drawable* lineVelocity = Utils::getVectorShape(getPosition(), getVelocity(), getShape()->getFillColor());
		//shapesToDraw.push_back(lineVelocity);

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
		std::cout << typeid(*rule).name() << " Force : " << Vector2::getMagnitude(weightedForce) << std::endl;
		applyForce(weightedForce);
	}

}
