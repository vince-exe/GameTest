#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace SettingsFileUtils {
	const std::string SETTINGS_PATH = "settings/settings.json";

	const std::string CLEAR_USELESS_THREADS_EACH = "ClearUselessThreadsEach";
}

class ServerSettings {
	ServerSettings() = default;
	~ServerSettings() = default;
	ServerSettings& operator=(const ServerSettings&) = delete; // Disable = operator
	ServerSettings& operator=(ServerSettings&&) = delete; // disable move operator

private:
	std::string m_settingsPath;
	rapidjson::Document m_Document;

private:
	bool createSettingsFile();

public:
	static ServerSettings& getInstance();

	bool init(const std::string& filePath);

	rapidjson::Value& getValue(const std::string& key);

	void setInt_(const std::string& key, int v);
};