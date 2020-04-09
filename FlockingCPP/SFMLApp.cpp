#include "SFMLApp.h"

#include <iostream>
#include "SFML/Window.hpp"
#include "Vector2Utilities.hpp"

using namespace Utils;

sf::Vector2f SFMLApp::getDirectionFromKeyboardInputs()
{

	sf::Vector2f direction;

	//Real-time input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		direction += Vector2::left();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		direction += Vector2::right();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		direction += Vector2::up();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		direction += Vector2::down();
	}

	direction = Vector2::normalized(direction);

	return direction;
}

void SFMLApp::warpParticleIfOutOfBounds(Particle& particle)
{
	//Correct position with windows borders
	sf::Vector2f position = particle.getShape().getPosition();
	sf::Vector2u sizeWindow = window_ptr->getSize();

	if (position.x < 0) {
		position.x += sizeWindow.x;
	}
	else if (position.x > sizeWindow.x) {
		position.x -= sizeWindow.x;
	}

	if (position.y < 0) {
		position.y += sizeWindow.y;
	}
	else if (position.y > sizeWindow.y) {
		position.y -= sizeWindow.y;
	}

	//If the position changed
	if (position != particle.getShape().getPosition())
	{
		particle.getShape().setPosition(position);
	}
}

int SFMLApp::run()
{
	/// Initialization windows & settings
	sf::ContextSettings settings;
	settings.antialiasingLevel = antialiasing;

	sf::RenderWindow window(sf::VideoMode(heightWindow, widthWindow), "Boids !", sf::Style::Default, settings);
	window.setFramerateLimit(maxFramerate);

	window_ptr = &window;
	
	int nbBoids = 10;
	float baseSpeed = 50.;
	for (int i = 0; i < nbBoids; i++) {

		//New boids with random starting positions
		Particle boid;
		boid.setPosition(Vector2::getRandom(heightWindow, widthWindow));
		boid.setVelocity(Vector2::getVector2FromDegree(rand() % 180) * baseSpeed); //Random dir

		boids.push_back(boid);
	}

	sf::Clock deltaClock;

	/// MAIN LOOP
	while (window.isOpen())
	{
		//Time between each frames
		sf::Time deltaTime = deltaClock.restart();

		///EVENTS
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		///REALTIME INPUTS
		sf::Vector2f axisInput = getDirectionFromKeyboardInputs();

		if (axisInput != sf::Vector2f()) 
		{
			boids[0].applyForce(axisInput * 5.f);
		}

		///UPDATE LOGIC

		//update each boid logic
		for (std::vector<Particle>::iterator it = boids.begin(); it != boids.end(); it++) {
			it->update(deltaTime.asSeconds());
		}

		/* Update logic and position are separated so the movement calculations don't 
		take into account the new positions of other boids */

		//Update their position
		for (std::vector<Particle>::iterator it = boids.begin(); it != boids.end(); it++) {
			it->updatePosition(deltaTime.asSeconds());
			warpParticleIfOutOfBounds(*it);
		}
		

		///DRAW SCENE

		window.clear();

		//Draw each boid
		for (std::vector<Particle>::iterator it = boids.begin(); it != boids.end(); it++) {
			window.draw(it->getShape());
		}

		window.display();
	}

	return 0;
}
