#include "Boid.h"
#include "Vector2Utilities.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

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
			std::cout << "distance : " << distance << std::endl;
			//Verify if boid is close enough to be part of the neighborhood
			if (distance <= detectionRadius)
			{
				neighborhood.push_back(boid);
			}
		}
		
	}

	return neighborhood;
}

sf::Vector2f Boid::separationForce(const std::vector<Boid*>& neighbordhood)
{

	//Try to avoid boids too close
	sf::Vector2f separatingForce;

	for (std::vector<Boid*>::const_iterator it = neighbordhood.begin(); it != neighbordhood.end(); it++)
	{
		Boid* flockmate = (*it);
		float distance = Vector2::getDistance(getPosition(), flockmate->getPosition());
		sf::Vector2f direction = Vector2::normalized(flockmate->getPosition() - getPosition());

		//Each neighbor has an influence proportional to its distance
		if (distance > 0) {
			separatingForce += -direction / distance;
		}
	}

	if (neighbordhood.size() > 0)
	{
		separatingForce /= static_cast<float>(neighbordhood.size());
	}

	return separatingForce;
}

sf::Vector2f Boid::alignmentForce(const std::vector<Boid*> &neighbordhood)
{
	//Try to match the heading and speed of neighbors = Average velocity

	sf::Vector2f averageVelocity;

	for (std::vector<Boid*>::const_iterator it = neighbordhood.begin(); it != neighbordhood.end(); it++)
	{
		averageVelocity += (*it)->getVelocity();
	}

	if (neighbordhood.size() > 0)
	{
		averageVelocity /= static_cast<float>(neighbordhood.size());
	}

	return averageVelocity;
}

sf::Vector2f Boid::cohesionForce(const std::vector<Boid*> &neighbordhood)
{

	sf::Vector2f centerOfMass;

	for (std::vector<Boid*>::const_iterator it = neighbordhood.begin(); it != neighbordhood.end(); it++)
	{
		centerOfMass += (*it)->getPosition();
	}

	if (neighbordhood.size() > 0)
	{
		centerOfMass /= static_cast<float>(neighbordhood.size());
	}

	//Get direction toward center of mass
	sf::Vector2f towardCenter = centerOfMass - getPosition();

	//normalize?

	return towardCenter;
}

void Boid::update(const float deltaTime)
{
	Particle::update(deltaTime);

	std::vector<Boid*> neighbordhood = computeBoidNeighbordhood();

	sf::Vector2f force = cohesionWeight * cohesionForce(neighbordhood);

	std::cout << "Force : " << Vector2::getMagnitude(force) << std::endl;
	applyForce(force);

	force = separationWeight * separationForce(neighbordhood);
	std::cout << "Force : " << Vector2::getMagnitude(force) << std::endl;
	applyForce(force);

	force = alignmentWeight * alignmentForce(neighbordhood);
	std::cout << "Force : " << Vector2::getMagnitude(force) << std::endl;
	applyForce(force);

	//std::cout << "NeighborCount : " << neighbordhood.size() << std::endl;

}
