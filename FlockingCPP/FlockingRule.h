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

	FlockingRule(sf::Color debugColor_, float weight_, bool isEnabled_ = true) :
		debugColor(debugColor_),
		force(sf::Vector2f()),
		weight(weight_),
		isEnabled(isEnabled_)
	{}

	virtual sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) = 0;

	virtual float getBaseWeightMultiplier() 
	{
		return 1.;
	};

	virtual const char* getRuleName() = 0;

	virtual const char* getRuleExplanation() = 0;

public:

	float weight;

	bool isEnabled;

	//Copy constructor
	FlockingRule(const FlockingRule& toCopy);

	virtual std::unique_ptr<FlockingRule> clone() = 0;


	sf::Vector2f computeWeightedForce(const std::vector<Boid*>& neighbordhood, Boid* boid);

	virtual bool drawImguiRule();


	// Inherited via Drawable
	virtual void draw(const Boid &boid, sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

};

/* Steer to move toward center of mass of local flockmates */
class CohesionRule : public FlockingRule 
{

public:

	CohesionRule(float weight = 1., bool isEnabled = true) : FlockingRule(sf::Color::Cyan, weight, isEnabled)  {}

	std::unique_ptr<FlockingRule> clone() override
	{
		// Créer un pointeur concret en utilisant le constructeur abstrait parent
		return std::make_unique<CohesionRule>(*this);
	}

	const char* getRuleName() override
	{
		return "Cohesion Rule";
	}

	const char* getRuleExplanation() override
	{
		return "Steer to move toward center of mass of nearby boids.";
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

private:

	float desiredMinimalDistance = 10;

public:

	SeparationRule(float desiredSeparation = 20., float weight = 1., bool isEnabled = true) : 
		FlockingRule(sf::Color::Red, weight, isEnabled), desiredMinimalDistance(desiredSeparation) 
	{}

	std::unique_ptr<FlockingRule> clone() override
	{
		return std::make_unique<SeparationRule>(*this);
	}

	const char* getRuleName() override
	{
		return "Separation Rule";
	}

	const char* getRuleExplanation() override
	{
		return "Steer to avoid collision with nearby boids.";
	}

	virtual float getBaseWeightMultiplier() override
	{
		return 100.;
	}


	sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) override;

	bool drawImguiRule() override;

};

/* Steer toward average heading of local flockmates */
class AlignmentRule : public FlockingRule
{

public:

	AlignmentRule(float weight = 1., bool isEnabled = true) : FlockingRule(sf::Color::Yellow, weight, isEnabled) {}

	

	std::unique_ptr<FlockingRule> clone() override
	{
		return std::make_unique<AlignmentRule>(*this);
	}

	const char* getRuleName() override
	{
		return "Alignment Rule";
	}	

	const char* getRuleExplanation() override
	{
		return "Steer to move in the same direction that nearby boids.";
	}

	virtual float getBaseWeightMultiplier() override
	{
		return .01;
	}

	sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) override;
};

class WindRule : public FlockingRule
{
private:

	float windAngle;

public:

	WindRule(float weight = 1., float angle = 0, bool isEnabled = true) : FlockingRule(sf::Color::White, weight, isEnabled), windAngle(angle)
	{}

	WindRule(const WindRule & toCopy) : FlockingRule(toCopy)
	{
		windAngle = toCopy.windAngle;
	}


	std::unique_ptr<FlockingRule> clone() override
	{
		return std::make_unique<WindRule>(*this);
	}

	const char* getRuleName() override
	{
		return "Wind Force";
	}

	const char* getRuleExplanation() override
	{
		return "Apply a constant force to all boids.";
	}

	virtual float getBaseWeightMultiplier() override
	{
		return 100.;
	}

	sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) override;

	bool drawImguiRule() override;
};

class MouseInfluenceRule : public FlockingRule
{

private:

	//If not avoiding, is attracted
	bool isRepulsive;



public:

	MouseInfluenceRule(float weight = 1., bool isRepulsive_ = false, bool isEnabled = true) : FlockingRule(sf::Color::Magenta, weight, isEnabled), isRepulsive(isRepulsive_)
	{}

	MouseInfluenceRule(const MouseInfluenceRule& toCopy) : FlockingRule(toCopy)
	{
		isRepulsive = toCopy.isRepulsive;
	}

	std::unique_ptr<FlockingRule> clone() override
	{
		return std::make_unique<MouseInfluenceRule>(*this);
	}


	const char* getRuleName() override
	{
		return "Mouse Click Influence";
	}

	const char* getRuleExplanation() override
	{
		return "Steer toward or away the mouse when clicked.";
	}

	virtual float getBaseWeightMultiplier() override
	{
		return 0.1;
	}


	sf::Vector2f computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid) override;

	bool drawImguiRule() override;
public:

};

