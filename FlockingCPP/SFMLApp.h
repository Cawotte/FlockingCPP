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

	int nbBoids = 60;
	float baseSpeed = 100.;

	//Members
	sf::RenderWindow* window_ptr;
	std::vector<Particle*> particles;

	//Configuration
	std::map<FlockingRule*, bool*> boidsRules;
	float* defaultWeights; //array of default rules weight
	float detectionRadius = 40.f;
	bool showRadius = true;
	bool showRuleVectors = true;

	//Data
	//Time between each frames
	sf::Time deltaTime;

	///Methods
	sf::Vector2f getDirectionFromKeyboardInputs();

	std::vector<FlockingRule*> getFlockingRules();

	void warpParticleIfOutOfBounds(Particle& particle);

	void showConfigurationWindow();
	void showMemoryInfo();
	//void showPlotDataGraph(utils::vector::PlotData<float>& data);

	void applyConfigurationToAllBoids();
	void setNumberOfBoids(int number);
	void randomizeBoidPositionAndVelocity(Boid& boid);


	std::vector<Boid*> getAllBoids();

public:

	SFMLApp() {};

	int run();

};

