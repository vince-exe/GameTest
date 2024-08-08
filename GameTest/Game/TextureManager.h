#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class TextureManager {
public:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager& operator=(const TextureManager&) = delete; // Disable = operator
	TextureManager& operator=(TextureManager&&) = delete; // disable move operator

private:
	// buttons 
	sf::Texture m_quitBtnTexture, m_settingsBtnTexture, m_settings2BtnTexture, m_undoMatchTexture, m_matchBtnTexture, m_connectBtnTexture, m_doneBtn, m_cancelBtn;
	// texts images
	sf::Texture m_textsImages[9];

public:
	bool init();
	
    const sf::Texture& getQuitBtnTexture() const;

	const sf::Texture& getSettingsBtnTexture() const;

	const sf::Texture& getSettings2BtnTexture() const;

	const sf::Texture& getUndoMatchTexture() const;

	const sf::Texture& getMatchBtnTexture() const;

	const sf::Texture& getConnectBtnTexture() const;

	const sf::Texture& getDoneBtn() const;

	const sf::Texture& getCancelBtn() const;

	const sf::Texture& getTextImage(unsigned int index) const;
};