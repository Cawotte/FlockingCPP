
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Boid.h"
#include "Vector2Utilities.hpp"


using namespace Utils;

//Start of the program
int main()
{

	//std::cout << "Hello World!";

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!", sf::Style::Default, settings);
	//sf::CircleShape shape(30.f);

	Boid boid;

	float speed = 3.f;

	window.setFramerateLimit(60);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}

		sf::Vector2f movement;

		//Real-time input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			movement += Vector2::left();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			movement += Vector2::right();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			movement += Vector2::up();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			movement += Vector2::down();
		}



		if (movement != sf::Vector2f()) {
			std::cout << Utils::Vector2::getMagnitude(movement) << std::endl;
			movement = Utils::Vector2::normalized<sf::Vector2f>(movement);

			boid.getShape().setRotation(Vector2::getAngleDegree(movement));
			boid.setVelocity(movement * speed);
		}

		boid.update(window);

		window.clear();
		window.draw(boid.getShape());
		window.display();
	}

	return 0;
}
