#include "AudioManager.h"

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

bool AudioManager::init() {
    m_soundEffectsVolume = 0;

    return 
        m_backgroundMusic.openFromFile(SkyfallUtils::Audio::BACKGROUND_MUSIC_PATH) && m_matchmakingSound.openFromFile(SkyfallUtils::Audio::MATCHMAKING_SOUND)
        && m_buttonClick.openFromFile(SkyfallUtils::Audio::BUTTON_CLICK_SOUND) && m_errorSound.openFromFile(SkyfallUtils::Audio::ERROR_SOUND);
}

Music& AudioManager::getBackgroundMusic() {
    return m_backgroundMusic;
}

Sound& AudioManager::getMatchmakingSound() {
    return m_matchmakingSound;
}

Sound& AudioManager::getButtonClickSound() {
    return m_buttonClick;
}

Sound& AudioManager::getErrorSound() {
    return m_errorSound;
}

int AudioManager::getSoundEffectsVolume() {
    return m_soundEffectsVolume;
}

void AudioManager::setSoundEffectsVolume(int vol) {
    m_soundEffectsVolume = vol;

    m_matchmakingSound.setVolume(vol);
    m_buttonClick.setVolume(vol);
    m_errorSound.setVolume(vol);
}