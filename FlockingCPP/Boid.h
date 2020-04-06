#pragma once


#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Vector2Utilities.hpp"

class Boid
{

private:

	//Default values
	static const float default_size;
	static const sf::Color default_color;

	//Members
	float speed = 5.;

	//-- References

	//-- Attributes
	sf::CircleShape shape;

	sf::Vector2f velocity;

	//Methods
	void warpBoidIfOutOfBounds(const sf::RenderWindow& window);
public:

	//Constructor

	Boid(float size = default_size, sf::Color color = default_color, 
		sf::Vector2f position = sf::Vector2f(), sf::Vector2f velocity = sf::Vector2f());

	//Getter / Setters
	sf::CircleShape& getShape() {
		return shape;
	}

	void setPosition(sf::Vector2f position) {
		shape.setPosition(position);
	}

	void setVelocity(sf::Vector2f velocity) {
		this->velocity = velocity;

		shape.setRotation(Utils::Vector2::getAngleDegree(velocity));
	}

	//Methods
	void update(const sf::RenderWindow& window);


};

