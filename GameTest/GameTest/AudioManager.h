#pragma once

#include "utils.h"
#include "Music.h"
#include "Sound.h"

class AudioManager {
private:
	AudioManager() = default;
	~AudioManager() = default;
	AudioManager& operator=(const AudioManager&) = delete; // disable = operator
	AudioManager& operator=(AudioManager&&) = delete; // disable move operator

private:
	Music m_backgroundMusic, m_battleMusic;
	Sound m_matchmakingSound , m_buttonClick, m_errorSound, m_countdownSound;
	int m_soundEffectsVolume;

public:
	static AudioManager& getInstance();

	bool init();

	Music& getBackgroundMusic();
	
	Music& getBattleMusic();

	Sound& getMatchmakingSound();

	Sound& getButtonClickSound();

	Sound& getErrorSound();

	Sound& getCountdownSound();

	int getSoundEffectsVolume();

	void setSoundEffectsVolume(int vol);
};
