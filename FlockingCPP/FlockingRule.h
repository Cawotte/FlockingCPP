#pragma once


#include "SFML/Graphics.hpp"

class Boid;

class FlockingRule
{

protected:

	sf::Vector2f force;

	//if displayed
	sf::Color debugColor;

	FlockingRule(sf::Color debugColor_, float weight_) : debugColor(debugColor_), force(sf::Vector2f()), weight(weight_) {}

public:

	float weight;

	sf::Vector2f getForce() 
	{
		return force;
	}

	sf::Vector2f getWeightedForce()
	{
		return force * weight;
	}

	sf::Drawable* getVectorShape(Boid* boid);

	virtual sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) = 0;

	sf::Vector2f computeWeightedForce(const std::vector<Boid*>& neighbordhood, Boid* boid);


};

/* Steer to move toward center of mass of local flockmates */
class CohesionRule : public FlockingRule 
{

public:

	CohesionRule(float weight = 1.) : FlockingRule(sf::Color::Cyan, weight)  {}

	sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) override;
};

/* Steer to avoid local flockmates */
class SeparationRule : public FlockingRule
{

public:

	SeparationRule(float weight = 1.) : FlockingRule(sf::Color::Red, weight) {}

	sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) override;
};

/* Steer toward average heading of local flockmates */
class AlignmentRule : public FlockingRule
{

public:

	AlignmentRule(float weight = 1.) : FlockingRule(sf::Color::Yellow, weight) {}

	sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) override;
};

