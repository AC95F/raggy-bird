#include <SFML/Graphics.hpp>
#include "constants.h"
#include "pipe.h"

Pipe::Pipe(float gapPos, const sf::Texture &texture) {
	upperPart.setTexture(texture);
	upperPartHitbox.setFillColor(sf::Color::Green);

	lowerPart.setTexture(texture);
	lowerPartHitbox.setFillColor(sf::Color::Green);

	upperPart.setScale(1.5f, -1.5f);
	upperPartHitbox.setSize(sf::Vector2(upperPart.getLocalBounds().width, upperPart.getLocalBounds().height));
	upperPartHitbox.setScale(1.3f, -1.5f);
	upperPartHitbox.setOrigin(upperPartHitbox.getLocalBounds().width / 2, 0);

	lowerPart.setScale(1.5f, 1.5f);
	lowerPartHitbox.setSize(sf::Vector2(lowerPart.getLocalBounds().width, lowerPart.getLocalBounds().height));
	lowerPartHitbox.setScale(1.3f, 1.5f);
	lowerPartHitbox.setOrigin(lowerPartHitbox.getLocalBounds().width / 2, 0);

	lowerPart.setPosition(screenResolution.x, screenResolution.y * 0.58f + gapPos);
	lowerPartHitbox.setPosition(lowerPart.getPosition().x + lowerPart.getLocalBounds().width * 0.75f, lowerPart.getPosition().y);

	upperPart.setPosition(screenResolution.x, screenResolution.y * 0.32f + gapPos);
	upperPartHitbox.setPosition(upperPart.getPosition().x + upperPart.getLocalBounds().width * 0.75f, upperPart.getPosition().y);

	isPassed = false;
}

void Pipe::Move(float newPos) {
	lowerPart.setPosition(newPos, lowerPart.getPosition().y);
	lowerPartHitbox.setPosition(lowerPart.getPosition().x + lowerPart.getLocalBounds().width * 0.75f, lowerPart.getPosition().y);

	upperPart.setPosition(newPos, upperPart.getPosition().y);
	upperPartHitbox.setPosition(upperPart.getPosition().x + upperPart.getLocalBounds().width * 0.75f, upperPart.getPosition().y);
}

float Pipe::GetPosition() {
	return lowerPart.getPosition().x;
}

bool Pipe::IsOutOfBounds() {
	return (lowerPart.getPosition().x < 0 - lowerPart.getLocalBounds().width * 1.5f);
}