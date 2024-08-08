#include "ServerSettings.h"

bool ServerSettings::init(const std::filesystem::path& path) {
	m_settingsPath = std::make_shared<std::filesystem::path>(path);
    std::ifstream inputFile(path);

    if (!std::filesystem::exists(ServerUtils::Settings::SETTINGS_DIRECTORY)) {
        std::cout << "\nNo settings directory found...[ Created a new settings directory ] \n";
        std::filesystem::create_directory(ServerUtils::Settings::SETTINGS_DIRECTORY);
    }
    if (!inputFile.is_open()) {
        if (createSettingsFile()) {
            std::cerr << "\nNo settings file found...[ Created a new settings file ]\n";
            inputFile.open(path);
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
    
    writer.Key(ServerUtils::Settings::MAX_CONNECTIONS.c_str());
    writer.Int(10);

    writer.Key(ServerUtils::Settings::TCP_PORT.c_str());
    writer.Int(8888);

    writer.Key(ServerUtils::Settings::UDP_PORT.c_str());
    writer.Int(8889);

    writer.Key(ServerUtils::Settings::CLEAR_USELESS_THREADS.c_str());
    writer.Int(120);

    writer.Key(ServerUtils::Settings::UDP_REQUEST_TIMEOUT.c_str());
    writer.Int(5);

    writer.Key(ServerUtils::Settings::THREADS_NUMBER.c_str());
    writer.Int(10);

    writer.EndObject();

    std::ofstream outputFile(*m_settingsPath);
    if (outputFile.is_open()) {
        outputFile << s.GetString();
        outputFile.close();

        return true;
    }

    return false;
}