#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "utils.h"

class ServerSettings {
	ServerSettings() = default;
	~ServerSettings() = default;
	ServerSettings& operator=(const ServerSettings&) = delete; // Disable = operator
	ServerSettings& operator=(ServerSettings&&) = delete; // disable move operator

private:
	std::shared_ptr<std::filesystem::path> m_settingsPath;
	rapidjson::Document m_Document;

private:
	bool createSettingsFile();

public:
	static ServerSettings& getInstance();

	bool init(const std::filesystem::path& path);

	rapidjson::Value& getValue(const std::string& key);

	void setInt_(const std::string& key, int v);
};