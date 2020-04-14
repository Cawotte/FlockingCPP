#include "SFMLApp.h"

#include <iostream>
#include "SFML/Window.hpp"
#include "Utils.hpp"
#include "Boid.h"
#include "ImGuiExtra.h"

//Memory
#include "windows.h"
#include "psapi.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

using namespace utils;

sf::Vector2f SFMLApp::getDirectionFromKeyboardInputs()
{

	sf::Vector2f direction;

	//Real-time input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		direction += vector2::left();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		direction += vector2::right();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		direction += vector2::up();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		direction += vector2::down();
	}

	direction = vector2::normalized(direction);

	return direction;
}


void SFMLApp::warpParticleIfOutOfBounds(Particle& particle)
{
	//Correct position with windows borders
	sf::Vector2f position = particle.getShape()->getPosition();
	sf::Vector2u sizeWindow = window_ptr->getSize();

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

	//If the position changed
	if (position != particle.getShape()->getPosition())
	{
		particle.getShape()->setPosition(position);
	}
}

void SFMLApp::showConfigurationWindow()
{

	ImGui::Begin("Configuration"); // begin window

	ImGui::Text("Control the simulation with those settings.");
	ImGui::Spacing();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);

	if (ImGui::CollapsingHeader("General"))
	{

		if (ImGui::DragInt("Number of Boids", &nbBoids)) 
		{
			if (nbBoids < 0)
				nbBoids = 0;
			setNumberOfBoids(nbBoids);
		}

		if (ImGui::SliderFloat("Neighborhood Radius", &detectionRadius, 0.0f, 300.0f, "%.f"))
		{
			std::vector<Boid*> boids = getAllBoids();
			for (const auto& boid : boids)
			{
				boid->setDetectionRadius(detectionRadius);
			}
		}

		if (ImGui::Checkbox("Show Radius", &showRadius))
		{
			std::vector<Boid*> boids = getAllBoids();
			for (const auto& boid : boids)
			{
				boid->drawDebugRadius = showRadius;
			}
		}

		if (ImGui::Checkbox("Show Rules", &showRuleVectors))
		{
			std::vector<Boid*> boids = getAllBoids();
			for (const auto& boid : boids)
			{
				boid->drawDebugRules = showRuleVectors;
			}
		}

		if (ImGui::Button("Randomize Boids position and velocity"))
		{
			std::vector<Boid*> boids = getAllBoids();
			for (const auto& boid : boids)
			{
				randomizeBoidPositionAndVelocity(*boid);
			}
		}
	}

	if (ImGui::CollapsingHeader("Rules"))
	{
		int i = 0;
		for (auto& rule : boidsRules)
		{
			i++;
			ImGui::PushID(i);

			if (rule->drawImguiRule())
			{
				applyConfigurationToAllBoids();
			}
			ImGui::Separator();

			ImGui::PopID();
		}

		if (ImGui::Button("Restore Default Weights"))
		{
			int i = 0;
			//restore default values
			for (auto& rule : boidsRules)
			{
				rule->weight = defaultWeights[i++];
			}
			applyConfigurationToAllBoids();
		}

		ImGui::Spacing();
	}

	if (ImGui::CollapsingHeader("Performance"))
	{
		float framePerSecond = 1. / deltaTime.asSeconds();
		ImGui::Text("Frames Per Second (FPS) : %.f", framePerSecond);
		PlotVar("Frame duration (ms)", deltaTime.asMilliseconds());
		ImGui::Separator();
		showMemoryInfo();
	}

	ImGui::End(); // end window
}

void SFMLApp::showMemoryInfo()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);

	//Total Virtual Memory
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;

	//Virtual Memory currently used
	DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

	//Virtual Memory current process
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;

	//Total RAM
	DWORDLONG totalPhysMem = memInfo.ullTotalPhys;

	//Ram Currently Used
	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

	//Ram Current used by Process
	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;

	int div = 1048576;

	//PC info

	ImGui::Text("Total Virtual Memory : %uMb \n", totalVirtualMem / div);
	ImGui::Text("Total RAM : %uMb \n", totalPhysMem / div);

	//ImGui::Text("Virtual Memory Currently Used : %iMb \n", virtualMemUsed / div);
	//ImGui::Text("RAM Currently Used : %uMb \n", physMemUsed / div);
	//ImGui::Separator();

	ImGui::Text("Virtual Memory used by process : %uMb \n", virtualMemUsedByMe / div);
	PlotVar("Virtual Memory Consumption (Mb)", virtualMemUsedByMe / div);

	ImGui::Text("RAM used by process : %uMb \n", physMemUsedByMe / div);
	PlotVar("Ram Consumption (Mb)", physMemUsedByMe / div);
}


void SFMLApp::applyConfigurationToAllBoids()
{

	//Get Boids list
	std::vector<Boid*> boids = getAllBoids();

	//For each boid
	for (const auto& boid : boids)
	{
		boid->setFlockingRules(boidsRules);
	}

}

void SFMLApp::setNumberOfBoids(int number)
{

	int diff = particles.size() - number;

	if (diff == 0) 
	{
		return;
	}
	//Need to add boids
	else if (diff < 0)
	{
		//Back to positive
		diff = -diff;

		//Add boids equal to diff
		for (int i = 0; i < diff; i++)
		{
			//Create new boid
			Boid* boid = new Boid(&particles); 
			randomizeBoidPositionAndVelocity(*boid);
			boid->setFlockingRules(boidsRules);
			boid->drawDebugRadius = showRadius;
			boid->drawDebugRules = showRuleVectors;
			boid->setDetectionRadius(detectionRadius);

			particles.push_back(boid);
		}
	}
	//Too much boid, remove them
	else
	{
		//Remove from end
		for (int i = 0; i < diff; i++)
		{
			Particle* p = particles.back();
			particles.pop_back();

			Boid* boid = dynamic_cast<Boid*>(p);

			if (boid != nullptr) {

				delete boid; //clean memory
			}
		}
	
	}
}

void SFMLApp::randomizeBoidPositionAndVelocity(Boid& boid)
{

	boid.setPosition(vector2::getRandom(widthWindow, heightWindow));
	boid.setVelocity(vector2::getVector2FromDegree(rand() % 180) * baseSpeed); //Random dir
}

std::vector<Boid*> SFMLApp::getAllBoids()
{
	std::vector<Boid*> boids;

	for (const auto& p : particles)
	{
		Boid* boid = dynamic_cast<Boid*>(p);

		if (boid != nullptr) {
			boids.push_back(boid);
		}
	}

	return boids;
}

int SFMLApp::run()
{
	/// Initialization windows & settings
	sf::ContextSettings settings;
	settings.antialiasingLevel = antialiasing;

	sf::RenderWindow window(sf::VideoMode(widthWindow, heightWindow), "Boids !", sf::Style::Default, settings);
	window.setFramerateLimit(maxFramerate);
	ImGui::SFML::Init(window);

	window_ptr = &window;


	//INITIALIZE RULES

	boidsRules.emplace_back(new SeparationRule(4.50));
	boidsRules.emplace_back(new CohesionRule(8.));
	boidsRules.emplace_back(new AlignmentRule(6.));
	boidsRules.emplace_back(new WindRule(1., 0, false));
	boidsRules.emplace_back(new MouseInfluenceRule(1.));

	defaultWeights = new float[boidsRules.size()];
	int i = 0;
	//save default values
	for (auto& rule : boidsRules)
	{
		defaultWeights[i++] = rule->weight;
	}

	setNumberOfBoids(nbBoids);

	applyConfigurationToAllBoids();
	

	sf::Clock deltaClock;

	/// MAIN LOOP
	while (window.isOpen())
	{
		//Time between each frames
		deltaTime = deltaClock.restart();

		///EVENTS
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();
		}

		///REALTIME INPUTS
		sf::Vector2f axisInput = getDirectionFromKeyboardInputs();

		if (axisInput != sf::Vector2f()) 
		{
			particles[0]->applyForce(axisInput * 20.f);
		}

		///UPDATE LOGIC

		//update each boid logic
		for (auto& p : particles) {
			p->update(deltaTime.asSeconds());
		}

		/* Update logic and position are separated so the movement calculations don't 
		take into account the new positions of other boid */

		//Update their position
		for (auto& p : particles) {
			p->updatePosition(deltaTime.asSeconds());
			warpParticleIfOutOfBounds(*p);
		}

		///IMGUI UI

		ImGui::SFML::Update(window, deltaTime);

		ImGui::ShowDemoWindow();
		showConfigurationWindow();
		

		///DRAW SCENE

		window.clear();

		//Draw each boid
		for (auto& p : particles) {
			
			int i = 0;
			//The particle returns all their components to draw
			std::vector<sf::Drawable*> toDraw = p->toDraw();
			for (auto drawable : toDraw) {
				window.draw(*drawable);

				//Ignore the main shape. UGLY HACK
				if (i > 0) 
				{
					delete drawable;
				}
				i++;
			}
			toDraw.clear();
		}

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}
