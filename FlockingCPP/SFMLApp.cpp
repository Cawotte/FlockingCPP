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

int SFMLApp::run()
{
	/// Initialization windows & settings
	sf::ContextSettings settings;
	settings.antialiasingLevel = antialiasing;

	sf::RenderWindow window(sf::VideoMode(heightWindow, widthWindow), "Boids !", sf::Style::Default, settings);
	window.setFramerateLimit(maxFramerate);

	
	int nbBoids = 10;
	for (int i = 0; i < nbBoids; i++) {

		//New boids with random starting positions
		Boid boid;
		boid.setPosition(Vector2::getRandom(heightWindow, widthWindow));
		boid.setVelocity(Vector2::getVector2FromDegree(rand() % 180)); //Random dir

		boids.push_back(boid);
	}

	/// MAIN LOOP
	while (window.isOpen())
	{

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
			boids[0].setVelocity(axisInput);
		}

		///UPDATE LOGIC

		//update each boid
		for (std::vector<Boid>::iterator it = boids.begin(); it != boids.end(); it++) {
			it->update(window);
		}

		///DRAW SCENE

		window.clear();

		//Draw each boid
		for (std::vector<Boid>::iterator it = boids.begin(); it != boids.end(); it++) {
			window.draw(it->getShape());
		}

		window.display();
	}

	return 0;
}
