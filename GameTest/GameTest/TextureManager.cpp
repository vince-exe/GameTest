#include "TextureManager.h"

bool TextureManager::init() {
    // load the texs images
    for (int i = 0; i < 9; i++) {
        if (!m_textsImages[i].loadFromFile("assets/Texts-Messages/" + std::to_string(i) + ".png")) {
            return false;
        }
    }
    // load the buttons textures
    return
        m_quitBtnTexture.loadFromFile("assets/Buttons/quit.png") && m_settingsBtnTexture.loadFromFile("assets/Buttons/settings.png")
        && m_settings2BtnTexture.loadFromFile("assets/Buttons/settings2.png") && m_connectBtnTexture.loadFromFile("assets/Buttons/connect.png")
        && m_matchBtnTexture.loadFromFile("assets/Buttons/match.png") && m_undoMatchTexture.loadFromFile("assets/Buttons/undoMatch.png")
        && m_doneBtn.loadFromFile("assets/Buttons/done.png") && m_cancelBtn.loadFromFile("assets/Buttons/cancel.png");
}

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

const sf::Texture& TextureManager::getQuitBtnTexture() const {
    return m_quitBtnTexture;
}

const sf::Texture& TextureManager::getSettingsBtnTexture() const {
    return m_settingsBtnTexture;
}

const sf::Texture& TextureManager::getSettings2BtnTexture() const {
    return m_settings2BtnTexture;
}

const sf::Texture& TextureManager::getUndoMatchTexture() const {
    return m_undoMatchTexture;
}

const sf::Texture& TextureManager::getMatchBtnTexture() const {
    return m_matchBtnTexture;
}

const sf::Texture& TextureManager::getConnectBtnTexture() const {
    return m_connectBtnTexture;
}

const sf::Texture& TextureManager::getDoneBtn() const {
    return m_doneBtn;
}

const sf::Texture& TextureManager::getCancelBtn() const {
    return m_cancelBtn;
}

const sf::Texture& TextureManager::getTextImage(unsigned int index) const {
    return m_textsImages[index];
}
