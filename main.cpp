#include <SFML/Graphics.hpp>

int main()
{
	sf::Color backgroundColor(37, 31, 64);
	sf::Color shapeColor(214, 171, 219);

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Flappy Bird SFML");
	sf::CircleShape shape(20.f);
	shape.setFillColor(shapeColor);
	shape.setPosition(window.getSize().x / 4.f - shape.getLocalBounds().width / 2, 0);
	sf::Clock deltaClock;
	window.setKeyRepeatEnabled(false);

	const float gravity = 30.f;
	const float mass = 20.f;
	const float maxSpeed = 100.f;
	const float jumpForce = -20.f;

	float xVelocity = 0.f;
	float yVelocity = 0.f;

	while (window.isOpen())
	{
		sf::Time deltaTime = deltaClock.restart();
		sf::Event event;
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

		// Increase the speed
		yVelocity += gravity * deltaTime.asSeconds();
		if (yVelocity > maxSpeed)
		{
			yVelocity = maxSpeed;
		}

		shape.move(0, yVelocity * mass * deltaTime.asSeconds());

		// Reset shape position if out of bounds
		if (shape.getPosition().y + shape.getLocalBounds().width > window.getSize().y || shape.getPosition().y < 0.f)
		{
			shape.setPosition(window.getSize().x / 4.f - shape.getLocalBounds().width / 2, 0);
			//yVelocity = 0.f;
		}

		window.clear(backgroundColor);
		window.draw(shape);
		window.display();
	}

	return 0;
}