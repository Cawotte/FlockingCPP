#pragma once
#include "Particle.h"
#include "Utils.hpp"

class Boid : public Particle
{

private:

	//Reference to all particles

	std::vector<Particle*>* particles;

	float detectionRadius = 250.;

	//Weights
	float cohesionWeight = 1.f;
	float separationWeight = 1000.f;
	float alignmentWeight = 0.2f;

	//Methods
	std::vector<Boid*> computeBoidNeighbordhood();

	//Rules

	/* Steer to avoid local flockmates */
	sf::Vector2f separationForce(const std::vector<Boid*> &neighbordhood);

	/* Steer toward average heading of local flockmates */
	sf::Vector2f alignmentForce(const std::vector<Boid*> &neighbordhood);

	/* Steer to move toward center of mass of local flockmates */
	sf::Vector2f cohesionForce(const std::vector<Boid*> &neighbordhood);

public:

	Boid() : Particle() {}


	Boid(std::vector<Particle*>* particles_) : Particle(), particles(particles_) {}


	void update(const float deltaTime) override;

};

