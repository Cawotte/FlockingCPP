#include "SFMLApp.h"

#include <iostream>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Vector2Utilities.hpp"
#include "Boid.h"

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
	sf::Vector2f position = particle.getShape()->getPosition();
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
	if (position != particle.getShape()->getPosition())
	{
		particle.getShape()->setPosition(position);
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
	
	/*
	particles = std::vector<Particle*>(nbBoids);
	for (auto& particle : particles) 
	{
		Boid boid = Boid(&particles);
		boid.setPosition(Vector2::getRandom(heightWindow, widthWindow));
		boid.setVelocity(Vector2::getVector2FromDegree(rand() % 180) * baseSpeed); //Random dir

		*particle = boid;
	}*/


	particles = std::vector<Particle*>();
	particles.reserve(nbBoids);
	for (int i = 0; i < nbBoids; i++) {

		//New boids with random starting positions
		Boid* boid = new Boid(&particles);
		boid->setPosition(Vector2::getRandom(heightWindow, widthWindow));
		boid->setVelocity(Vector2::getVector2FromDegree(rand() % 180) * baseSpeed); //Random dir

		particles.push_back(boid);
	}

	sf::Clock deltaClock;

	std::cout << "Hellooo : " << particles.size() << std::endl;
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
			particles[0]->applyForce(axisInput * 5.f);
		}

		///UPDATE LOGIC

		//update each boid logic
		for (auto& p : particles) {
			p->update(deltaTime.asSeconds());
		}

		/* Update logic and position are separated so the movement calculations don't 
		take into account the new positions of other boid */

		//Update their position
		for (auto& p : particles) {
			p->updatePosition(deltaTime.asSeconds());
			warpParticleIfOutOfBounds(*p);
		}

		///DRAW SCENE

		window.clear();

		//Draw each boid
		for (auto& p : particles) {
			
			//The particle returns all their components to draw
			std::vector<sf::Drawable*> toDraw = p->toDraw();
			for (std::vector<sf::Drawable*>::iterator itd = toDraw.begin(); itd != toDraw.end(); itd++)
			{
				window.draw(**itd);
			} 
		}

		window.display();
	}

	return 0;
}
