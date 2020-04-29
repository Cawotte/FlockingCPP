#include "FlockingRule.h"

#include "Utils.hpp"
#include "Boid.h"
#include "imgui/imgui.h"
#include "ImGuiExtra.h"


using namespace utils;

FlockingRule::FlockingRule(const FlockingRule& toCopy)
{
	weight = toCopy.weight;
	debugColor = toCopy.debugColor;
	force = toCopy.force;
	isEnabled = toCopy.isEnabled;
}

sf::Vector2f FlockingRule::computeWeightedForce(const std::vector<Boid*>& neighbordhood, Boid* boid)
{
	//the computed force is cached in a var

	if (isEnabled)
	{
		force = getBaseWeightMultiplier() * weight * computeForce(neighbordhood, boid);
	}
	else 
	{
		//If the rule is not enabled, return vector zero.
		force = sf::Vector2f();
	}

	return force;
}

bool FlockingRule::drawImguiRule()
{
	bool valueHasChanged = false;

	//std::string rulename(typeid(this).name());
	//rulename = rulename.substr(6); //remove "class "

	ImGui::BulletText(getRuleName());
	ImguiTooltip(getRuleExplanation());
	if (ImGui::Checkbox("Enabled", &isEnabled))
	{
		valueHasChanged = true;
	}

	if (isEnabled)
	{
		if (ImGui::DragFloat("Weight##", &weight, 0.025f))
		{
			valueHasChanged = true;
		}

		ImGui::SameLine(); HelpMarker("Drag to change the weight's value or CTRL+Click to input a new value.");

	}

	return valueHasChanged;
}

void FlockingRule::draw(const Boid& boid, sf::RenderTarget& target, sf::RenderStates states) const
{
	graphics::drawVector(target, states, boid.getPosition(), force, debugColor);
}

sf::Vector2f CohesionRule::computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid)
{
	sf::Vector2f centerOfMass;

	for (std::vector<Boid*>::const_iterator it = neighbordhood.begin(); it != neighbordhood.end(); it++)
	{
		centerOfMass += (*it)->getPosition();
	}

	if (neighbordhood.size() > 0)
	{
		centerOfMass /= static_cast<float>(neighbordhood.size());

		//Get direction toward center of mass
		sf::Vector2f towardCenter = centerOfMass - boid->getPosition();

		force = vector2::normalized(towardCenter);
	}
	else 
	{
		force = sf::Vector2f();
	}

	return force;
}

sf::Vector2f SeparationRule::computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid)
{

	//Try to avoid boids too close
	sf::Vector2f separatingForce;

	for (std::vector<Boid*>::const_iterator it = neighbordhood.begin(); it != neighbordhood.end(); it++)
	{
		Boid* flockmate = (*it);
		float distance = vector2::getDistance(boid->getPosition(), flockmate->getPosition());

		if (distance < desiredMinimalDistance && distance > 0)
		{
			sf::Vector2f direction = vector2::normalized(flockmate->getPosition() - boid->getPosition());

			separatingForce += -direction / distance;
			//Each neighbor has an influence proportional to its distance
			//separatingForce += -direction / exp(distance / 1000.f);
		}
	}

	if (neighbordhood.size() > 0)
	{
		separatingForce /= static_cast<float>(neighbordhood.size());
	}

	force = separatingForce;
	//force = vector2::normalized(separatingForce);

	return separatingForce;
}

bool SeparationRule::drawImguiRule()
{
	bool valusHasChanged = FlockingRule::drawImguiRule();

	if (isEnabled)
	{

		if (ImGui::DragFloat("Desired Separation", &desiredMinimalDistance, 0.05f))
		{
			valusHasChanged = true;
		}

	}
	return valusHasChanged;
}

sf::Vector2f AlignmentRule::computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid)
{
	//Try to match the heading and speed of neighbors = Average velocity

	sf::Vector2f averageVelocity;

	for (std::vector<Boid*>::const_iterator it = neighbordhood.begin(); it != neighbordhood.end(); it++)
	{
		averageVelocity += (*it)->getVelocity();
	}

	if (neighbordhood.size() > 0)
	{
		averageVelocity /= static_cast<float>(neighbordhood.size());
	}

	force = averageVelocity;

	return averageVelocity;
}

///WIND 

sf::Vector2f WindRule::computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid)
{
	return utils::vector2::normalized(utils::vector2::getVector2FromRadian(windAngle));
}

bool WindRule::drawImguiRule()
{
	bool valusHasChanged = FlockingRule::drawImguiRule();

	if (isEnabled)
	{

		if (ImGui::SliderAngle("Wind Direction", &windAngle, 0))
		{
			valusHasChanged = true;
		}

	}
	return valusHasChanged;
}

/// MOUSE

sf::Vector2f MouseInfluenceRule::computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid)
{

	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::IsMousePosValid() && io.MouseDown[0])
	{
		sf::Vector2f mousePos(io.MousePos.x, io.MousePos.y);
		sf::Vector2f displacement = mousePos - boid->getPosition();
		float distance = utils::vector2::getMagnitude(displacement);

		//The force is inversely proportional to distance

		sf::Vector2f force = displacement / exp(distance / 100.f);

		if (isRepulsive)
		{
			force *= -1.f;
		}

		return force;
	}
	else 
	{
		return sf::Vector2f();
	}

}

bool MouseInfluenceRule::drawImguiRule()
{

	bool hasChangedValue = FlockingRule::drawImguiRule();

	if (isEnabled)
	{
		if (ImGui::RadioButton("Attractive", !isRepulsive))
		{
			isRepulsive = false;
			hasChangedValue = true;
		}

		ImGui::SameLine();
		if (ImGui::RadioButton("Repulsive", isRepulsive))
		{
			isRepulsive = true;
			hasChangedValue = true;
		}
	}

	return hasChangedValue;
}

sf::Vector2f BoundedAreaRule::computeForce(const std::vector<Boid*>& neighbordhood, Boid* boid)
{
	//Return a force proportional to the proximity of the boids with the bounds, and opposed to it

	sf::Vector2f force; //zero

	//Too close from min
	if (boid->getPosition().x < desiredDistance)
	{
		force.x += desiredDistance / boid->getPosition().x; //car position.x = distance de 0 à x.
	}
	//Too close from max
	else if (boid->getPosition().x > widthWindows - desiredDistance)
	{
		int d = boid->getPosition().x - widthWindows;
		if (d == 0) d = 1000; //avoid div by zero
		force.x += desiredDistance / d;
	}

	//Too close from min
	if (boid->getPosition().y < desiredDistance)
	{
		force.y += desiredDistance / boid->getPosition().y; 
	}
	//Too close from max
	else if (boid->getPosition().y > heightWindows - desiredDistance)
	{
		int d = boid->getPosition().y - heightWindows;
		if (d == 0) d = 1000; //avoid div by zero
		force.y += desiredDistance / d;
	}

	return force;
}

bool BoundedAreaRule::drawImguiRule()
{

	bool valueHasChanged = FlockingRule::drawImguiRule();
	if (isEnabled)
	{
		//We cap the max separation as the third of the min of the width.height
		int minHeightWidth = std::min(widthWindows, heightWindows);
		if (ImGui::SliderInt("Desired Distance From Borders", &desiredDistance, 0.0f, minHeightWidth / 3, "%i"))
		{
			valueHasChanged = true;
		}

	}
	return valueHasChanged;
}

void BoundedAreaRule::draw(const Boid& boid, sf::RenderTarget& target, sf::RenderStates states) const
{

	FlockingRule::draw(boid, target, states);

	//Draw a rectangle on the map

	sf::RectangleShape bound(sf::Vector2f(widthWindows - 2 * desiredDistance, heightWindows - 2 * desiredDistance));
	bound.setOutlineColor(debugColor);
	bound.setOutlineThickness(1); //1 pixel outline
	bound.setFillColor(sf::Color::Transparent); //Transparent inside, we just draw the box bound
	bound.setPosition(desiredDistance, desiredDistance);

	target.draw(bound, states);


}
