#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main();
sf::Sound LoadSound(std::string fileName, sf::SoundBuffer &buffer);