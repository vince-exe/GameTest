#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>

namespace SkyfallUtils {
	namespace Settings {
		const std::filesystem::path SETTINGS_PATH = "settings/settings.json";
		
		const std::filesystem::path SETTINGS_DIRECTORY = "settings";

		const std::string MUSIC_VOLUME = "MusicVolume";

		const std::string DEFAULT_NETWORK = "DefaultIpPort";

		const std::string SOUND_EFFECTS_VOLUME = "SoundEffectsVolume";

		const std::string DEBUG_MODE = "DebugMode";
	}
	namespace Audio {
		const std::filesystem::path BACKGROUND_MUSIC = "assets/Music-Sound/background_menu_music.ogg";
		
		const std::filesystem::path BUTTON_CLICK_SOUND = "assets/Music-Sound/button_click.ogg";

		const std::filesystem::path MATCHMAKING_SOUND = "assets/Music-Sound/matchmaking.ogg";

		const std::filesystem::path BATTLE_MUSIC = "assets/Music-Sound/battle_music.ogg";

		const std::filesystem::path COUNTDOWN_SOUND = "assets/Music-Sound/countdown.ogg";

		const std::filesystem::path ERROR_SOUND = "assets/Music-Sound/error_sound.ogg";
	}
	namespace Font {
		const std::filesystem::path FREDOKA_ONE = "assets/Fonts/FredokaOne-Regular.ttf";
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