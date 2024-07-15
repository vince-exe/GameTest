#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

namespace SkyfallUtils {
	namespace Settings {
		const std::string SETTINGS_PATH = "settings/settings.json";
		
		const std::string MUSIC_VOLUME = "MusicVolume";

		const std::string DEFAULT_NETWORK = "DefaultIpPort";

		const std::string SOUND_EFFECTS_VOLUME = "SoundEffectsVolume";

		const std::string DEBUG_MODE = "DebugMode";
	}
	namespace Audio {
		const std::string BACKGROUND_MUSIC_PATH = "assets/Music-Sound/background_menu_music.ogg";
		
		const std::string BUTTON_CLICK_SOUND = "assets/Music-Sound/button_click.ogg";

		const std::string MATCHMAKING_SOUND = "assets/Music-Sound/matchmaking.ogg";

		const std::string ERROR_SOUND = "assets/Music-Sound/error_sound.ogg";
	}

	/*
		the 3 value a window can return 
	*/
	enum class WindowsReturnValues {
		DONE,
		EXIT,
		BACK
	};

	bool doLinesIntersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4);

	bool doesRectangleIntersectLine(const sf::RectangleShape& rectangle, const sf::VertexArray& line);
}