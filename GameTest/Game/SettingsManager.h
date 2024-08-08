#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "utils.h"

class SettingsManager {
public:
	SettingsManager() = default;
	~SettingsManager() = default;
	SettingsManager& operator=(const SettingsManager&) = delete; // Disable = operator
	SettingsManager& operator=(SettingsManager&&) = delete; // disable move operator

private:
	std::shared_ptr<std::filesystem::path> m_settingsPath;
	rapidjson::Document m_Document;

private:
	bool createSettingsFile();

public:
	bool init(const std::filesystem::path& path);

	bool storeSettings();

	rapidjson::Value& getValue (const std::string& key);

	void setInt_(const std::string& key, int v);

	void setString_(const std::string& key, const std::string& v);
};
