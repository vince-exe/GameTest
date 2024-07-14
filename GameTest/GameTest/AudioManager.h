#pragma once

#include "utils.h"
#include "Music.h"
#include "Sound.h"

class AudioManager {
private:
	AudioManager() = default;
	~AudioManager() = default;
	AudioManager& operator=(const AudioManager&) = delete; // Disable = operator
	AudioManager& operator=(AudioManager&&) = delete; // disable move operator

private:
	Music m_backgroundMusic;
	Sound m_matchmakingSound;

public:
	static AudioManager& getInstance();

	bool init();

	Music& getBackgroundMusic();
	
	Sound& getMatchmakingSound();
};
