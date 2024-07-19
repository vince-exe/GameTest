#include "SettingsManager.h"

SettingsManager& SettingsManager::getInstance() {
    static SettingsManager singleton;
    return singleton;

}

bool SettingsManager::init(const std::string& filePath) {
    m_settingsPath = filePath;
    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        std::cerr << "\nNo settings file found...[ Creating a new settings file ]";
        
        if (createSettingsFile()) {
            /* successfully created the settings file*/
            inputFile.open(filePath);
        }
        else {
            return false;
        }
    }

    std::string json((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    m_Document.Parse(json.c_str());

    // Check for parse errors
    if (m_Document.HasParseError()) {
        std::cerr << "Error parsing JSON: " << m_Document.GetParseError() << std::endl;
        return false;
    }

    return true;
}

bool SettingsManager::createSettingsFile() {
    m_Document.SetObject();
    rapidjson::Document::AllocatorType& allocator = m_Document.GetAllocator();

    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);

    writer.StartObject();

    writer.Key(SkyfallUtils::Settings::MUSIC_VOLUME.c_str());
    writer.Int(10);

    writer.Key(SkyfallUtils::Settings::SOUND_EFFECTS_VOLUME.c_str());
    writer.Int(20);

    writer.Key(SkyfallUtils::Settings::DEBUG_MODE.c_str());
    writer.String("OFF");

    writer.Key(SkyfallUtils::Settings::DEFAULT_NETWORK.c_str());
    writer.String("127.0.0.1:8888");

    writer.EndObject();

    std::ofstream outputFile(m_settingsPath);
    if (outputFile.is_open()) {
        outputFile << s.GetString();
        outputFile.close();

        std::cout << "\nSuccessfully created the settings.json file in " << m_settingsPath << "\n";
        return true;
    }

    return false;
}

bool SettingsManager::storeSettings() {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    m_Document.Accept(writer);
    std::string jsonString = buffer.GetString();

    std::ofstream outputFile(m_settingsPath);
    if (outputFile.is_open()) {
        outputFile << jsonString;
        outputFile.close();

        return true;
    }
    
    return false;
}

rapidjson::Value& SettingsManager::getValue(const std::string& key) {
    auto it = m_Document.FindMember(key.c_str());

    if (it != m_Document.MemberEnd()) {
        return it->value;
    }

    throw std::runtime_error("Key not found");
}

void SettingsManager::setInt_(const std::string& key, int v) {
    auto it = m_Document.FindMember(key.c_str());

    if (it != m_Document.MemberEnd() && it->value.IsInt()) {
        it->value.SetInt(v);
    }
    else {
        throw std::runtime_error("Update error");
    }
}

void SettingsManager::setString_(const std::string& key, const std::string& v) {
    auto it = m_Document.FindMember(key.c_str());
    
    if (it != m_Document.MemberEnd() && it->value.IsString()) {
        it->value.SetString(v.c_str(), static_cast<rapidjson::SizeType>(v.length()), m_Document.GetAllocator());
    }
    else {
        throw std::runtime_error("Update error");
    }
}