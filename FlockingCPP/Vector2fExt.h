#pragma once

#include "SFML/System/Vector2.hpp"

class Vector2fExt : public sf::Vector2f
{
	static const sf::Vector2f Up;
	static const sf::Vector2f Down;
	static const sf::Vector2f Left;
	static const sf::Vector2f Right;



	sf::Vector2f normalized();
	float getMagnitude();

	float getAngleRadians();
	float getAngleDegrees();
};

