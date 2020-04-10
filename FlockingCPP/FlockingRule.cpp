#include "FlockingRule.h"

#include "Utils.hpp"
#include "Boid.h"


using namespace utils;

sf::Drawable* FlockingRule::getVectorShape(Boid* boid)
{
	return graphics::getVectorShape(boid->getPosition(), getWeightedForce(), debugColor);
}

sf::Vector2f FlockingRule::computeWeightedForce(const std::vector<Boid*>& neighbordhood, Boid* boid)
{
	return weight * computeForce(neighbordhood, boid);
}

sf::Vector2f CohesionRule::computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid)
{
	sf::Vector2f centerOfMass;

	for (std::vector<Boid*>::const_iterator it = neighbordhood.begin(); it != neighbordhood.end(); it++)
	{
		centerOfMass += (*it)->getPosition();
	}

	if (neighbordhood.size() > 0)
	{
		centerOfMass /= static_cast<float>(neighbordhood.size());

		//Get direction toward center of mass
		sf::Vector2f towardCenter = centerOfMass - boid->getPosition();

		force = towardCenter;
	}
	else 
	{
		force = sf::Vector2f();
	}

	return force;
}

sf::Vector2f SeparationRule::computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid)
{
	//Try to avoid boids too close
	sf::Vector2f separatingForce;

	for (std::vector<Boid*>::const_iterator it = neighbordhood.begin(); it != neighbordhood.end(); it++)
	{
		Boid* flockmate = (*it);
		float distance = vector2::getDistance(boid->getPosition(), flockmate->getPosition());
		sf::Vector2f direction = vector2::normalized(flockmate->getPosition() - boid->getPosition());

		//Each neighbor has an influence proportional to its distance
		if (distance > 0) {
			separatingForce += -direction / distance;
		}
	}

	if (neighbordhood.size() > 0)
	{
		separatingForce /= static_cast<float>(neighbordhood.size());
	}

	force = separatingForce;

	return separatingForce;
}

sf::Vector2f AlignmentRule::computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid)
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

	force = averageVelocity;

	return averageVelocity;
}
