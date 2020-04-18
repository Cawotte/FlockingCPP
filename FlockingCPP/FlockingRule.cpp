#include "FlockingRule.h"

#include "Utils.hpp"
#include "Boid.h"
#include "imgui.h"
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

		force = towardCenter;
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
		sf::Vector2f direction = vector2::normalized(flockmate->getPosition() - boid->getPosition());

		//Each neighbor has an influence proportional to its distance
		if (distance > 0) {
			separatingForce += -direction / exp(distance / 1000.f);
		}
	}

	if (neighbordhood.size() > 0)
	{
		separatingForce /= static_cast<float>(neighbordhood.size());
	}

	force = separatingForce;

	return separatingForce;
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
