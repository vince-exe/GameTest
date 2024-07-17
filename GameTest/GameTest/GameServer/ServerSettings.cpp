#include "ServerSettings.h"

ServerSettings& ServerSettings::getInstance() {
	static ServerSettings instance;
	return instance;
}

bool ServerSettings::init(const std::string& filePath) {
	m_settingsPath = filePath;

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "\nNo file found [ Creating a new settings file ]";
        return createSettingsFile();
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

rapidjson::Value& ServerSettings::getValue(const std::string& key) {
    auto it = m_Document.FindMember(key.c_str());

    if (it != m_Document.MemberEnd()) {
        return it->value;
    }

    throw std::runtime_error("Key not found");
}

void ServerSettings::setInt_(const std::string& key, int v) {
    auto it = m_Document.FindMember(key.c_str());

    if (it != m_Document.MemberEnd() && it->value.IsInt()) {
        it->value.SetInt(v);
    }
    else {
        throw std::runtime_error("Update error");
    }
}

bool ServerSettings::createSettingsFile() {
    m_Document.SetObject();
    rapidjson::Document::AllocatorType& allocator = m_Document.GetAllocator();

    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);

    writer.StartObject();

    writer.Key("ClearUselessThreadsEach");
    writer.Int(1);

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