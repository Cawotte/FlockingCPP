#include "FlockingRule.h"

#include "Utils.hpp"
#include "Boid.h"
#include "imgui.h"


using namespace utils;

sf::Drawable* FlockingRule::getVectorShape(Boid* boid)
{
	return graphics::getVectorShape(boid->getPosition(), force, debugColor);
}

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
	if (ImGui::Checkbox("Enabled", &isEnabled))
	{
		//When the value is changed
		//Update rules
		valueHasChanged = true;
	}

	//ImGui::SliderFloat("##", &rule.first->weight, 0.0f, 100.0f, "weight = %.2f");

	if (ImGui::DragFloat("Weight##", &weight, 0.05f))
	{
		valueHasChanged = true;
	}


	//Rule name and ID

	//ImGui::PopID();

	return valueHasChanged;
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
			separatingForce += -direction / distance;
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
