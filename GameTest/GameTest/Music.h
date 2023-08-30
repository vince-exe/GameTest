#pragma once

#include <SFML/Audio.hpp>
#include <iostream>

class Music {
private:
	sf::Music music;
	int currentVolume;

public:
	Music() = default;

	bool openFromFile(const std::string& path);

	sf::Music& getMusic();

	void setVolume(float v);

	float getVolume();

	void play();

	void loop(bool loop);
};

