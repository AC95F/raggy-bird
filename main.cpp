#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "constants.h"
#include "main.h"
#include "pipe.h"

int main()
{
	sf::Texture birdTexture, birdTexture2, birdTexture3, pipeTexture, bgTexture, floorTexture;
	sf::Sprite bg, playerBird, floor;
	sf::CircleShape birdHitbox;

	sf::RenderWindow window(sf::VideoMode(screenResolution.x, screenResolution.y), "Raggy Bird");
	sf::Time deltaTime;
	sf::Clock deltaClock, pipeTimer, birdAnimTimer;
	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(true);

	int score = 0;

	sf::SoundBuffer scoreSoundBuffer;
	sf::Sound scoreSound = LoadSound("assets/sounds/score1.wav", scoreSoundBuffer);

	sf::SoundBuffer flapSoundBuffer;
	sf::Sound flapSound = LoadSound("assets/sounds/flap1.wav", flapSoundBuffer);

	sf::SoundBuffer deathSoundBuffer;
	sf::Sound deathSound = LoadSound("assets/sounds/death1.wav", deathSoundBuffer);

	bool isGameOver = false;
	bool isGameStarted = false;
	bool isBirdInGround = false;

	float gravity = 34.f;
	float mass = 20.f;
	float maxSpeed = 30.f;
	float jumpForce = -20.f;
	float yVelocity = 0.f;
	float birdAnimSpeed = 0.12f;
	float fallSpriteSpeed = 5.f;

	sf::Font font;
	font.loadFromFile("assets/fonts/upheavtt.ttf");
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setString("0");
	scoreText.setCharacterSize(60);
	scoreText.setPosition((screenResolution.x / 2) - scoreText.getLocalBounds().width / 2, 10);
	scoreText.setOutlineColor(sf::Color::Black);
	scoreText.setOutlineThickness(3.f);

	sf::Text titleText;
	titleText.setFont(font);
	titleText.setString("Raggy\n Bird");
	titleText.setCharacterSize(100);
	titleText.setPosition((screenResolution.x / 2) - titleText.getLocalBounds().width / 2, 10);
	titleText.setFillColor(sf::Color::White);
	titleText.setOutlineColor(sf::Color::Black);
	titleText.setOutlineThickness(3.f);

	sf::Text tutoText;
	tutoText.setFont(font);
	tutoText.setString("Press Space to Flap");
	tutoText.setCharacterSize(27);
	tutoText.setPosition((screenResolution.x / 2) - tutoText.getLocalBounds().width / 2, screenResolution.y / 1.5f);
	tutoText.setFillColor(sf::Color::White);

	sf::RectangleShape restartButton(sf::Vector2(100.f, 50.f));
	restartButton.setOrigin(restartButton.getLocalBounds().width / 2.f, restartButton.getLocalBounds().height / 2.f);
	restartButton.setPosition(screenResolution.x / 2.f, screenResolution.y / 1.5f);

	sf::Text restartButtonText;
	restartButtonText.setFont(font);
	restartButtonText.setString("Restart");
	restartButtonText.setCharacterSize(22);
	restartButtonText.setOrigin(restartButtonText.getLocalBounds().width / 2.f, restartButtonText.getLocalBounds().height / 2.f);
	restartButtonText.setPosition(restartButton.getPosition().x, restartButton.getPosition().y - restartButtonText.getLocalBounds().height);
	restartButtonText.setFillColor(sf::Color::Black);

	float pipeSpeed = 144.f;
	float timeBetweenPipes = 1.5f;
	std::vector<Pipe> pipes;
	std::random_device rd;
	std::mt19937 mt(rd());

	sf::Image icon;
	icon.loadFromFile("assets/sprites/bird_icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	if (bgTexture.loadFromFile("assets/sprites/bg.jpg")) {
		bg.setTexture(bgTexture);
	}
	if (floorTexture.loadFromFile("assets/sprites/floor.jpg")) {
		floor.setTexture(floorTexture);
		floor.setScale(1.5f, 1.5f);
	}

	pipeTexture.loadFromFile("assets/sprites/pipe.png");

	birdTexture.loadFromFile("assets/sprites/bird1.png");
	birdTexture2.loadFromFile("assets/sprites/bird2.png");
	birdTexture3.loadFromFile("assets/sprites/bird3.png");

	playerBird.setTexture(birdTexture);
	playerBird.setScale(1.5f, 1.5f);

	playerBird.setOrigin(playerBird.getLocalBounds().width / 2.f, playerBird.getLocalBounds().height / 2.f);
	playerBird.setPosition(screenResolution.x / 4.f, screenResolution.y / 2.f);
	floor.setPosition(0.f, screenResolution.y * 0.75f);
	
	birdHitbox.setRadius(playerBird.getLocalBounds().width / 2.f);
	birdHitbox.setFillColor(sf::Color::Green);
	birdHitbox.setOrigin(birdHitbox.getLocalBounds().width / 2.f, birdHitbox.getLocalBounds().height / 2.f);
	birdHitbox.setPosition(playerBird.getPosition());
	
	// Game Loop
	while (window.isOpen()) {
		deltaTime = deltaClock.restart();
		sf::Event event;

		// Player Input
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space && !isGameOver) {
					yVelocity = jumpForce;
					flapSound.play();
					if (!isGameStarted) isGameStarted = true;
				}
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left &&
				restartButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)
				&& isGameOver) {
					isGameOver = false;
					isGameStarted = false;
					isBirdInGround = false;
					playerBird.setPosition(screenResolution.x / 4.f, screenResolution.y / 2.f);
					yVelocity = 0.f;
					playerBird.setRotation(0.f);
					pipes.clear();
					score = 0;
				}
			}
		}

		// Update

		if (isGameStarted) {

			// Player Movement
			yVelocity += gravity * deltaTime.asSeconds();
			if (yVelocity > maxSpeed)
			{
				yVelocity = maxSpeed;
			}
			if (isGameOver && birdHitbox.getPosition().y >= floor.getPosition().y + 1.f) {
				yVelocity = 0.f;
				isBirdInGround = true;
			}

			playerBird.move(0, yVelocity * mass * deltaTime.asSeconds());

			//Out of bounds
			if (playerBird.getPosition().y + playerBird.getLocalBounds().width > window.getSize().y || playerBird.getPosition().y < 0.f)
			{
				playerBird.setPosition(screenResolution.x / 4.f, 0);
				yVelocity = 0.f;
			}
			
			birdHitbox.setPosition(playerBird.getPosition());

			if (!isBirdInGround) {
				playerBird.setRotation(yVelocity * 3.f);
			}

			// Pipe generation
			if (pipeTimer.getElapsedTime().asSeconds() >= timeBetweenPipes) {
				std::uniform_real_distribution<float> dist(-150.f, 50.f);
				pipes.push_back(Pipe(dist(mt), &pipeTexture));
				pipeTimer.restart();
			}

			// Pipe movement
			if (!isGameOver){
				for (size_t i = 0; i < pipes.size(); i++) {
					pipes[i].Move(pipes[i].GetPosition() - pipeSpeed * deltaTime.asSeconds());

					if (pipes[i].GetPosition() <= playerBird.getPosition().x && !pipes[i].isPassed) {
						score++;
						scoreSound.play();
						pipes[i].isPassed = true;
					}
					if (pipes[i].IsOutOfBounds()) {
						pipes.erase(pipes.begin()+i);
						i--;
					}
					if (birdHitbox.getGlobalBounds().intersects(pipes[i].lowerPartHitbox.getGlobalBounds())
					|| birdHitbox.getGlobalBounds().intersects(pipes[i].upperPartHitbox.getGlobalBounds())
					|| birdHitbox.getGlobalBounds().intersects(floor.getGlobalBounds())
					) {
						isGameOver = true;
						yVelocity = jumpForce / 2.f;
						deathSound.play();
					}
				}
			}
			scoreText.setString(std::to_string(score));
		}

		// Floor movement
		if (!isGameOver) {
			floor.setPosition(floor.getPosition().x - pipeSpeed * deltaTime.asSeconds(), floor.getPosition().y);
			if (floor.getPosition().x <= -floor.getLocalBounds().width / 2.f) {
				floor.setPosition(0, floor.getPosition().y);
			}
		}

		// Bird animation
		if (birdAnimTimer.getElapsedTime().asSeconds() < birdAnimSpeed || yVelocity > fallSpriteSpeed || isGameOver) {
			playerBird.setTexture(birdTexture);
		}
		else if (birdAnimTimer.getElapsedTime().asSeconds() < birdAnimSpeed * 2) {
			playerBird.setTexture(birdTexture3);
		}
		else if (birdAnimTimer.getElapsedTime().asSeconds() < birdAnimSpeed * 3) {
			playerBird.setTexture(birdTexture2);
		}
		else {
			birdAnimTimer.restart();
		}

		// Button indication
		if (restartButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
			restartButton.setFillColor(sf::Color(200, 200, 200));
		}
		else {
			restartButton.setFillColor(sf::Color::White);
		}

		// Rendering
		window.clear();
		window.draw(bg);
		for (size_t i = 0; i < pipes.size(); i++) {
			window.draw(pipes[i].upperPart);
			window.draw(pipes[i].lowerPart);

			//window.draw(pipes[i].upperPartHitbox);
			//window.draw(pipes[i].lowerPartHitbox);
		}
		window.draw(playerBird);
		//window.draw(birdHitbox);
		window.draw(floor);
		if (isGameStarted) window.draw(scoreText);
		if (!isGameStarted) {
			window.draw(titleText);
			window.draw(tutoText);
		}
		if (isGameOver) {
			window.draw(restartButton);
			window.draw(restartButtonText);
		}
		window.display();
	}

	return 0;
}

sf::Sound LoadSound(std::string fileName, sf::SoundBuffer &buffer) {
	buffer.loadFromFile(fileName);

	sf::Sound sound;
	sound.setBuffer(buffer);

	return sound;
}