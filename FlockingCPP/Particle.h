#pragma once


#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Vector2Utilities.hpp"

class Particle
{

private:

	//Members
	float maxSpeed = 150.;

	sf::CircleShape shape;

	sf::Vector2f velocity; //Init at zero already
	sf::Vector2f acceleration;

	//Methods

	void resetAcceleration();

protected:


public:

	//Constructor

	Particle(float size = 8.f, sf::Color color = sf::Color::Green);

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
	void applyForce(sf::Vector2f force);


	virtual void update(const float deltaTime);

	virtual void updatePosition(const float deltaTime);



};

