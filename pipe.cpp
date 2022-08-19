#include <SFML/Graphics.hpp>
#include "constants.h"
#include "pipe.h"

Pipe::Pipe(float gapPos, const sf::Texture *texture) {
	upperPart.setTexture(*texture);
	lowerPart.setTexture(*texture);

	upperPart.setScale(1.5f, -1.5f);
	lowerPart.setScale(1.5f, 1.5f);

	//upperPart.rotate(180.f);

	lowerPart.setPosition(screenResolution.x, screenResolution.y * 0.58f + gapPos);
	upperPart.setPosition(screenResolution.x, screenResolution.y * 0.32f + gapPos);
}

void Pipe::Move(float newPos) {
	lowerPart.setPosition(newPos, lowerPart.getPosition().y);
	upperPart.setPosition(newPos, upperPart.getPosition().y);
}

float Pipe::GetPosition() {
	return lowerPart.getPosition().x;
}

bool Pipe::IsOutOfBounds() {
	return (lowerPart.getPosition().x < 0 - lowerPart.getLocalBounds().width * 1.5f);
}