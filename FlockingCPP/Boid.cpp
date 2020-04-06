#include "Boid.h"


//Default values
const float Boid::default_size = 8.f;
const sf::Color Boid::default_color = sf::Color::Green;


Boid::Boid(float size, sf::Color color, sf::Vector2f position, sf::Vector2f velocity)
{
	this->shape = sf::CircleShape(size, 3);
	shape.setFillColor(color);
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition(position);

	this->velocity = velocity;
}


//Public Methods

void Boid::update(const sf::RenderWindow& window)
{
	shape.move(velocity * speed);

	warpBoidIfOutOfBounds(window);

}

//Private Methods

void Boid::warpBoidIfOutOfBounds(const sf::RenderWindow& window)
{
	//Correct position with windows borders
	sf::Vector2f position = shape.getPosition();
	sf::Vector2u sizeWindow = window.getSize();

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

	shape.setPosition(position);
}

