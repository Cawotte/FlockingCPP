#include "Utils.hpp"

sf::Drawable* utils::graphics::getVectorShape(const sf::Vector2f start, const sf::Vector2f vector, const sf::Color color) 
{
	sf::VertexArray* line = new sf::VertexArray(sf::Lines, 2);
	(*line)[0] = start;
	(*line)[1] = start + vector;

	(*line)[0].color = color;
	(*line)[1].color = color;

	return line;
}