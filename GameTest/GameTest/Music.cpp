#include "Music.h"

bool Music::openFromFile(const std::string& path) {
    return m_Music.openFromFile(path);
}

sf::Music& Music::getMusic() {
    return m_Music;
}

void Music::setVolume(float v) {
    m_currentVolume = v;
    m_Music.setVolume(v);
}

float Music::getVolume() {
    return m_currentVolume;
}

void Music::play() {
    m_Music.play();
}

void Music::loop(bool loop) {
    m_Music.setLoop(loop);
}
