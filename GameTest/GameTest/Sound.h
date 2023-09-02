#pragma once

#include <SFML/Audio.hpp>

class Sound {
public:
	Sound() = default;

	bool openFromFile(const std::string& path);

	sf::Sound& getSound();

	void setVolume(float v);

	float getVolume();

	void play();

private:
	sf::SoundBuffer soundBuff;
	sf::Sound sound;
};

