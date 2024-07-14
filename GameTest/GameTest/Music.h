#pragma once

#include <SFML/Audio.hpp>
#include <iostream>

class Music {
private:
	sf::Music m_Music;
	int m_currentVolume;

public:
	Music() = default;

	bool openFromFile(const std::string& path);

	const sf::Music& getMusic() const;

	void setVolume(float v);

	const float getVolume() const;

	void play();

	void loop(bool loop);

	void stop();
};