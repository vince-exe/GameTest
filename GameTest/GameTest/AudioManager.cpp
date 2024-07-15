#include "AudioManager.h"

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

bool AudioManager::init() {
    return 
        m_backgroundMusic.openFromFile(SkyfallUtils::Audio::BACKGROUND_MUSIC_PATH) && m_matchmakingSound.openFromFile(SkyfallUtils::Audio::MATCHMAKING_SOUND)
        && m_buttonClick.openFromFile(SkyfallUtils::Audio::BUTTON_CLICK_SOUND);
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
