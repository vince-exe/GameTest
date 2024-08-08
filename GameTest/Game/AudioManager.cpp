#include "AudioManager.h"

bool AudioManager::init() {
    m_soundEffectsVolume = 0;

    return 
        m_backgroundMusic.openFromFile(SkyfallUtils::Audio::BACKGROUND_MUSIC.string()) && m_matchmakingSound.openFromFile(SkyfallUtils::Audio::MATCHMAKING_SOUND.string())
        && m_buttonClick.openFromFile(SkyfallUtils::Audio::BUTTON_CLICK_SOUND.string()) && m_errorSound.openFromFile(SkyfallUtils::Audio::ERROR_SOUND.string())
        && m_battleMusic.openFromFile(SkyfallUtils::Audio::BATTLE_MUSIC.string()) && m_countdownSound.openFromFile(SkyfallUtils::Audio::COUNTDOWN_SOUND.string());
}

Music& AudioManager::getBackgroundMusic() {
    return m_backgroundMusic;
}

Music& AudioManager::getBattleMusic() {
    return m_battleMusic;
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

Sound& AudioManager::getCountdownSound() {
    return m_countdownSound;
}

int AudioManager::getSoundEffectsVolume() {
    return m_soundEffectsVolume;
}

void AudioManager::setSoundEffectsVolume(int vol) {
    m_soundEffectsVolume = vol;

    m_matchmakingSound.setVolume(vol);
    m_buttonClick.setVolume(vol);
    m_errorSound.setVolume(vol);
    m_countdownSound.setVolume(vol);
}