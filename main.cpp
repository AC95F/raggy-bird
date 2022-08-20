#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include "constants.h"
#include "pipe.h"

int main()
{
	sf::Texture birdTexture, pipeTexture, bgTexture, floorTexture;
	sf::Sprite bg, playerBird, floor;
	sf::CircleShape birdHitbox;

	sf::RenderWindow window(sf::VideoMode(screenResolution.x, screenResolution.y), "Raggy Bird");
	sf::Time deltaTime;
	sf::Clock deltaClock;
	sf::Clock pipeTimer;
	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(true);

	int score = 0;

	sf::Font font;
	font.loadFromFile("assets/fonts/upheavtt.ttf");
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setString("0");
	scoreText.setCharacterSize(60);
	scoreText.setPosition((screenResolution.x / 2) - scoreText.getLocalBounds().width / 2, 10);
	scoreText.setOutlineColor(sf::Color::Black);
	scoreText.setOutlineThickness(3.f);

	const float gravity = 34.f;
	const float mass = 20.f;
	const float maxSpeed = 100.f;
	const float jumpForce = -20.f;

	float xVelocity = 0.f;
	float yVelocity = 0.f;

	float pipeSpeed = 144.f;
	float timeBetweenPipes = 1.5f;
	std::vector<Pipe> pipes;
	std::random_device rd;

	if (bgTexture.loadFromFile("assets/sprites/bg.jpg")) {
		bg.setTexture(bgTexture);
	}
	if (birdTexture.loadFromFile("assets/sprites/bird.png")) {
		playerBird.setTexture(birdTexture);
		playerBird.setScale(1.5f, 1.5f);
	}
	if (floorTexture.loadFromFile("assets/sprites/floor.jpg")) {
		floor.setTexture(floorTexture);
		floor.setScale(1.5f, 1.5f);
	}

	pipeTexture.loadFromFile("assets/sprites/pipe.png");

	playerBird.setOrigin(playerBird.getLocalBounds().width / 2.f, playerBird.getLocalBounds().height / 2.f);
	playerBird.setPosition(window.getSize().x / 4.f - playerBird.getLocalBounds().width / 2, 0);
	floor.setPosition(0.f, screenResolution.y * 0.75f);
	
	birdHitbox.setRadius(playerBird.getLocalBounds().width / 2.f);
	birdHitbox.setFillColor(sf::Color::Green);
	birdHitbox.setOrigin(birdHitbox.getLocalBounds().width / 2.f, birdHitbox.getLocalBounds().height / 2.f);
	birdHitbox.setPosition(playerBird.getPosition());
	

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
				if (event.key.code == sf::Keyboard::Space)
				{
					yVelocity = jumpForce;
				}
			}
		}

		// Update

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
		
		playerBird.setRotation(yVelocity * 3.f);

		birdHitbox.setPosition(playerBird.getPosition());

		// Pipe generation
		if (pipeTimer.getElapsedTime().asSeconds() >= timeBetweenPipes) {
			std::mt19937 mt(rd());
			std::uniform_real_distribution<float> dist(-150.f, 50.f);
			pipes.push_back(Pipe(dist(mt), &pipeTexture));
			pipeTimer.restart();
		}

		// Pipe movement
		for (size_t i = 0; i < pipes.size(); i++) {
			pipes[i].Move(pipes[i].GetPosition() - pipeSpeed * deltaTime.asSeconds());

			if (pipes[i].GetPosition() <= playerBird.getPosition().x && !pipes[i].isPassed) {
				score++;
				pipes[i].isPassed = true;
			}
			if (pipes[i].IsOutOfBounds()) {
				pipes.erase(pipes.begin()+i);
				i--;
			}
			if (birdHitbox.getGlobalBounds().intersects(pipes[i].lowerPartHitbox.getGlobalBounds())
				|| birdHitbox.getGlobalBounds().intersects(pipes[i].upperPartHitbox.getGlobalBounds())
				|| birdHitbox.getGlobalBounds().intersects(floor.getGlobalBounds())) {
				score = 0;
			}
		}
		scoreText.setString(std::to_string(score));

		// Floor movement
		floor.setPosition(floor.getPosition().x - pipeSpeed * deltaTime.asSeconds(), floor.getPosition().y);
		if (floor.getPosition().x <= -floor.getLocalBounds().width / 2.f) {
			floor.setPosition(0, floor.getPosition().y);
		}

		// Rendering
		window.clear();
		window.draw(bg);
		for (size_t i = 0; i < pipes.size(); i++) {
			window.draw(pipes[i].upperPart);
			//window.draw(pipes[i].upperPartHitbox);

			window.draw(pipes[i].lowerPart);
			//window.draw(pipes[i].lowerPartHitbox);
		}
		window.draw(playerBird);
		//window.draw(birdHitbox);
		window.draw(floor);
		window.draw(scoreText);
		window.display();
	}

	return 0;
}