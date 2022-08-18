#pragma once

class Pipe {
public:
	sf::Sprite upperPart;
	sf::Sprite lowerPart;

	Pipe(float gapPos, const sf::Texture *texture);
	void Move(float newPos);
	float GetPosition();
	bool IsOutOfBounds();
};