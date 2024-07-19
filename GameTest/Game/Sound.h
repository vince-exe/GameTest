#pragma once

#include <SFML/Audio.hpp>

class Sound {
private:
	sf::SoundBuffer m_soundBuff;
	sf::Sound m_sound;

public:
	Sound() = default;

	bool openFromFile(const std::string& path);

	sf::Sound& getSound();

	void setVolume(float v);

	float getVolume();

	void play();

	void stop();
};