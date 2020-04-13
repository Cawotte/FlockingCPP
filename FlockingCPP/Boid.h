#pragma once
#include "Particle.h"

class FlockingRule;

class Boid : public Particle
{

private:

	//Reference to all particles

	std::vector<Particle*>* particles;

	float detectionRadius = 100.;

	std::vector<FlockingRule*> rules;

	//Methods
	std::vector<Boid*> computeBoidNeighbordhood();


public:

	//Member
	bool drawDebugRadius = true;
	bool drawDebugRules = true;

	//Constructor
	Boid(std::vector<Particle*>* particles_);

	//Getter - Setters
	void setFlockingRules(std::vector<FlockingRule*> newRules)
	{
		rules = newRules;
	}

	void setDetectionRadius(float newRadius)
	{
		detectionRadius = newRadius;
	}


	//Methods
	std::vector<sf::Drawable*> toDraw() override;

	void update(const float deltaTime) override;


};

