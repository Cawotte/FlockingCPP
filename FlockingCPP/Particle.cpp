#include "Particle.h"


Particle::Particle(float size, sf::Color color)
{
	this->shape = sf::CircleShape(size, 3);
	shape.setFillColor(color);
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition(sf::Vector2f());

	velocity = sf::Vector2f();
	acceleration = sf::Vector2f();

}


//Public Methods

std::vector<sf::Drawable*> Particle::toDraw()
{
	std::vector<sf::Drawable*> drawables;

	drawables.push_back(getShape());

	return drawables;
}

void Particle::applyForce(sf::Vector2f force)
{
	acceleration += force;
}

void Particle::update(const float deltaTime)
{

	//Compute acceleration

}

void Particle::updatePosition(const float deltaTime)
{
	//Apply acceleration to velocity
	setVelocity(velocity + acceleration);

	resetAcceleration();

	//If velocity too high, cap it
	if (Utils::Vector2::getMagnitude(velocity) > maxSpeed) 
	{
		setVelocity(Utils::Vector2::normalized(velocity) * maxSpeed);
	}

	shape.move(velocity * deltaTime);
}

//Private Methods



void Particle::resetAcceleration()
{
	acceleration = Utils::Vector2::zero();
}
