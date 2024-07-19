#include "Music.h"

bool Music::openFromFile(const std::string& path) {
    return m_Music.openFromFile(path);
}
 
const sf::Music& Music::getMusic() const {
    return m_Music;
}

void Music::setVolume(float v) {
    m_currentVolume = v;
    m_Music.setVolume(v);
}

const float Music::getVolume() const {
    return m_currentVolume;
}

void Music::play() {
    m_Music.play();
}

void Music::loop(bool loop) {
    m_Music.setLoop(loop);
}

void Music::stop() {
    m_Music.stop();
}