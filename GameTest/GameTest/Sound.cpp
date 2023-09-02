#include "Sound.h"

bool Sound::openFromFile(const std::string& path) {
    if (!this->soundBuff.loadFromFile(path)) {
        return false;
    }
    this->sound.setBuffer(this->soundBuff);
    return true;
}

sf::Sound& Sound::getSound() {
    return this->sound;
}

void Sound::setVolume(float v) {
    this->sound.setVolume(v);
}

float Sound::getVolume() {
    return this->sound.getVolume();
}

void Sound::play() {
    this->sound.play();
}
