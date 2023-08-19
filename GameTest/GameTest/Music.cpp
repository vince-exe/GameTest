#include "Music.h"

sf::Music& Music::getMusic() {
    return this->music;
}

void Music::setVolume(float v) {
    this->currentVolume = v;
    this->music.setVolume(v);
}

float Music::getVolume() {
    return this->currentVolume;
}

void Music::play() {
    this->music.play();
}

void Music::loop(bool loop) {
    this->music.setLoop(loop);
}
