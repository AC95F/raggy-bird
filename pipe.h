#pragma once

class Pipe {
public:
	sf::Sprite upperPart;
	sf::RectangleShape upperPartHitbox;

	sf::Sprite lowerPart;
	sf::RectangleShape lowerPartHitbox;
	
	bool isPassed;

	Pipe(float gapPos, const sf::Texture *texture);
	void Move(float newPos);
	float GetPosition();
	bool IsOutOfBounds();
};