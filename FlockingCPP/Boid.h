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

	bool drawDebug = false;

	Boid(std::vector<Particle*>* particles_);

	std::vector<sf::Drawable*> toDraw() override;

	void update(const float deltaTime) override;


};

