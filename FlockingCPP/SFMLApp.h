#pragma once


#include "SFML/Graphics.hpp"

#include "FlockingRule.h"
#include "Utils.hpp"

class Particle;


class SFMLApp
{


private:

	//Window Settings
	const int antialiasing = 8;
	const int maxFramerate = 60;

	const int widthWindow = 1200;
	const int heightWindow = 600;

	int nbBoids = 120;
	float baseSpeed = 100.;

	//Members
	sf::RenderWindow* window_ptr;
	std::vector<Particle*> particles;

	//Configuration
	std::vector<FlockingRule*> boidsRules;
	float* defaultWeights; //array of default rules weight
	float maxSpeed = 90.f;
	float detectionRadius = 40.f;
	bool showRadius = false;
	bool showRuleVectors = true;

	//Data
	//Time between each frames
	sf::Time deltaTime;

	///Methods
	sf::Vector2f getDirectionFromKeyboardInputs();

	//Warp the particle at the other side of screen if out of bounds
	void warpParticleIfOutOfBounds(Particle& particle);

	//ImGui
	void showConfigurationWindow();
	void showMemoryInfo();

	//Boids
	void applyConfigurationToAllBoids();
	void setNumberOfBoids(int number);
	void randomizeBoidPositionAndVelocity(Boid& boid);


	std::vector<Boid*> getAllBoids();

public:

	SFMLApp() {};

	int run();

};

