#pragma once


#include "SFML/Graphics.hpp"

class Boid;

class FlockingRule
{

protected:

	//Multiplier for weight so we can tilt values closer to each other
	sf::Vector2f force;

	//if displayed
	sf::Color debugColor;

	FlockingRule(sf::Color debugColor_, float weight_) :
		debugColor(debugColor_),
		force(sf::Vector2f()),
		weight(weight_)
	{}

	virtual sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) = 0;

	virtual float getBaseWeightMultiplier() 
	{
		return 1.;
	};

	virtual const char* getRuleName() = 0;

public:

	float weight;

	bool isEnabled = true;

	sf::Drawable* getVectorShape(Boid* boid);

	//Copy constructor
	FlockingRule(const FlockingRule& toCopy);

	virtual FlockingRule* clone() = 0;


	sf::Vector2f computeWeightedForce(const std::vector<Boid*>& neighbordhood, Boid* boid);

	virtual bool drawImguiRule();


};

/* Steer to move toward center of mass of local flockmates */
class CohesionRule : public FlockingRule 
{

public:

	CohesionRule(float weight = 1.) : FlockingRule(sf::Color::Cyan, weight)  {}

	FlockingRule* clone() override 
	{
		return new CohesionRule(*this);
	}

	const char* getRuleName() override
	{
		return "Cohesion Rule";
	}

	virtual float getBaseWeightMultiplier() override
	{
		return .1;
	}

	sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) override;
};

/* Steer to avoid local flockmates */
class SeparationRule : public FlockingRule
{

public:

	SeparationRule(float weight = 1.) : FlockingRule(sf::Color::Red, weight) {}

	FlockingRule* clone() override
	{
		return new SeparationRule(*this);
	}

	const char* getRuleName() override
	{
		return "Separation Rule";
	}

	virtual float getBaseWeightMultiplier() override
	{
		return 100.;
	}

	sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) override;
};

/* Steer toward average heading of local flockmates */
class AlignmentRule : public FlockingRule
{

public:

	AlignmentRule(float weight = 1.) : FlockingRule(sf::Color::Yellow, weight) {}

	FlockingRule* clone() override
	{
		return new AlignmentRule(*this);
	}

	const char* getRuleName() override
	{
		return "Alignment Rule";
	}	
	
	virtual float getBaseWeightMultiplier() override
	{
		return .01;
	}

	sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) override;
};

