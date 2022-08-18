#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include "constants.h"
#include "pipe.h"

int main()
{
	sf::Texture birdTexture;
	sf::Texture pipeTexture;
	sf::Texture bgTexture;
	sf::Sprite bg;

	sf::RenderWindow window(sf::VideoMode(screenResolution.x, screenResolution.y), "Raggy Bird");
	sf::Sprite playerBird;
	playerBird.setPosition(window.getSize().x / 4.f - playerBird.getLocalBounds().width / 2, 0);
	sf::Time deltaTime;
	sf::Clock deltaClock;
	sf::Clock pipeTimer;
	window.setKeyRepeatEnabled(false);

	const float gravity = 30.f;
	const float mass = 20.f;
	const float maxSpeed = 100.f;
	const float jumpForce = -20.f;

	float xVelocity = 0.f;
	float yVelocity = 0.f;

	float pipeSpeed = 150.f;
	float timeBetweenPipes = 1.5f;
	std::vector<Pipe> pipes;
	std::random_device rd;

	if (bgTexture.loadFromFile("assets/sprites/bg.jpg")) {
		bg.setTexture(bgTexture);
	}
	pipeTexture.loadFromFile("assets/sprites/pipe.png");
	if (birdTexture.loadFromFile("assets/sprites/bird.png")) {
		playerBird.setTexture(birdTexture);
		playerBird.setScale(1.5f, 1.5f);
	}

	//Pipe testPipe = Pipe(screenResolution.y / 2, &pipeTexture);

	// Game Loop
	while (window.isOpen())
	{
		deltaTime = deltaClock.restart();
		sf::Event event;

		// Player Input
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				yVelocity = jumpForce;
			}
		}

		// Player Movement
		yVelocity += gravity * deltaTime.asSeconds();
		if (yVelocity > maxSpeed)
		{
			yVelocity = maxSpeed;
		}

		playerBird.move(0, yVelocity * mass * deltaTime.asSeconds());

		if (playerBird.getPosition().y + playerBird.getLocalBounds().width > window.getSize().y || playerBird.getPosition().y < 0.f)
		{
			playerBird.setPosition(window.getSize().x / 4.f - playerBird.getLocalBounds().width / 2, 0);
			yVelocity = 0.f;
		}

		// Pipe generation
		if (pipeTimer.getElapsedTime().asSeconds() >= timeBetweenPipes) {
			std::mt19937 mt(rd());
			std::uniform_real_distribution<float> dist(-100.f, 100.f);
			pipes.push_back(Pipe(dist(mt), &pipeTexture));
			pipeTimer.restart();
		}

		for (size_t i = 0; i < pipes.size(); i++) {
			pipes[i].Move(pipes[i].GetPosition() - pipeSpeed * deltaTime.asSeconds());
			if (pipes[i].IsOutOfBounds()) {
				pipes.erase(pipes.begin()+i);
				i--;
			}
		}

		// Rendering
		window.clear();
		window.draw(bg);
		for (size_t i = 0; i < pipes.size(); i++) {
			window.draw(pipes[i].upperPart);
			window.draw(pipes[i].lowerPart);
		}
		window.draw(playerBird);
		window.display();
	}

	return 0;
}