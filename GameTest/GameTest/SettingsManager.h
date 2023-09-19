#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class SettingsManager {
private:
	static const std::string SETTINGS_PATH;
	static rapidjson::Document document;

private:
	static const char* jsonToString();

	static bool createSettingsFile();

public:
	static bool init();

	static bool storeSettings();

	static rapidjson::Value& getValue (const std::string& key);

	static void setInt_(const std::string& key, int v);

	static void setString_(const std::string& key, std::string v);
};
