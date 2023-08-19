#pragma once

#include <SFML/Audio.hpp>

class Music {
private:
	sf::Music music;
	int currentVolume;

public:
	Music() = default;

	sf::Music& getMusic();

	void setVolume(float v);

	float getVolume();

	void play();

	void loop(bool loop);
};

