#pragma once
#include "Particle.h"
#include "FlockingRule.h"

class Boid : public Particle
{

private:

	//Reference to all particles

	std::vector<Particle*>* particles;

	float detectionRadius = 100.;

	std::vector<std::unique_ptr<FlockingRule>> rules;

	//Methods
	std::vector<Boid*> computeBoidNeighbordhood();


public:

	//Member
	bool drawDebugRadius = true;
	bool drawDebugRules = true;

	//Constructor
	Boid(std::vector<Particle*>* particles_);

	//Getter - Setters
	void setFlockingRules(std::vector<std::unique_ptr<FlockingRule>> const& newRules)
	{
		rules.clear();

		//Clone the rules in newRules in the boid rules.

		for (auto& rule : newRules)
		{
			rules.push_back(rule->clone());
		}
	}

	void setDetectionRadius(float newRadius)
	{
		detectionRadius = newRadius;
	}

	void update(const float deltaTime) override;


	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

};

