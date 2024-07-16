#include "Sound.h"

bool Sound::openFromFile(const std::string& path) {
    if (!m_soundBuff.loadFromFile(path)) {
        return false;
    }
    m_sound.setBuffer(m_soundBuff);
    return true;
}

sf::Sound& Sound::getSound() {
    return m_sound;
}

void Sound::setVolume(float v) {
    m_sound.setVolume(v);
}

float Sound::getVolume() {
    return m_sound.getVolume();
}

void Sound::play() {
    m_sound.play();
}

void Sound::stop() {
    m_sound.stop();
}
