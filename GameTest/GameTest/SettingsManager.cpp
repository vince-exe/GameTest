#include "SettingsManager.h"

rapidjson::Document SettingsManager::document;
const std::string SettingsManager::SETTINGS_PATH = "settings/settings.json";

bool SettingsManager::createSettingsFile() {
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);

    writer.StartObject();

    writer.Key("VolumeMenu");
    writer.Int(20);

    writer.Key("DefaultIpPort");
    writer.String("127.0.0.1:8888");

    writer.EndObject();

    std::ofstream outputFile(SETTINGS_PATH);
    if (outputFile.is_open()) {
        outputFile << s.GetString();
        outputFile.close();

        std::cout << "\nSuccessfully created the settings.json file in " << SETTINGS_PATH << "\n";
        return true;
    }

    return false;
}

bool SettingsManager::storeSettings() {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    std::string jsonString = buffer.GetString();

    std::ofstream outputFile(SETTINGS_PATH);
    if (outputFile.is_open()) {
        outputFile << jsonString;
        outputFile.close();

        return true;
    }
    else {
        return false;
    }
}

bool SettingsManager::init() {
    std::ifstream inputFile(SETTINGS_PATH);
    if (!inputFile.is_open()) {
        std::cout << "\nNo settings file found.\n";
        return createSettingsFile();
    }

    std::string json((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    document.Parse(json.c_str());

    // Check for parse errors
    if (document.HasParseError()) {
        std::cout << "Error parsing JSON: " << document.GetParseError() << std::endl;
        return false;
    }

    return true;
}
