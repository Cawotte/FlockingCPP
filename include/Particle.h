#pragma once


#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Utils.hpp"

class Particle : public sf::Drawable
{

private:

	//Members
	float maxSpeed = 120.;


	sf::Vector2f velocity;
	sf::Vector2f acceleration;

	//Methods
	void resetAcceleration();

protected:

	sf::CircleShape shape;

public:

	//Constructor

	Particle(float size = 4.f, sf::Color color = sf::Color::Green);

	//Getter / Setters
	sf::CircleShape getShape() const {
		return shape;
	}

	sf::Vector2f getPosition() const {
		return shape.getPosition();
	}

	sf::Vector2f getVelocity() const {
		return velocity;
	}

	void setPosition(sf::Vector2f position) {
		shape.setPosition(position);
	}

	void setVelocity(sf::Vector2f velocity_) {
		velocity = velocity_;

		shape.setRotation(utils::vector2::getAngleDegree(velocity));
	}

	void setMaxSpeed(float newMaxSpeed)
	{
		maxSpeed = newMaxSpeed;
	}

	//Methods

	void applyForce(sf::Vector2f force);

	virtual void update(const float deltaTime);

	virtual void updatePosition(const float deltaTime);


	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

};

