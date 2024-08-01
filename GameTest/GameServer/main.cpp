#include <iostream>
	
#include "Server.h"
#include "utils.h"
#include "ServerSettings.h"

int main() {
	ServerSettings& sSingleton = ServerSettings::getInstance();
	if (!sSingleton.init(ServerUtils::Settings::SETTINGS_PATH)) {
		std::cerr << "\n [ ERROR ]: Failed to load the settings file";
		return 1;
	}

	Server server(sSingleton.getValue(ServerUtils::Settings::TCP_PORT).GetInt(), sSingleton.getValue(ServerUtils::Settings::UDP_PORT).GetInt(), sSingleton.getValue(ServerUtils::Settings::MAX_CONNECTIONS).GetInt(), sSingleton.getValue(ServerUtils::Settings::CLEAR_USELESS_THREADS).GetInt(), sSingleton.getValue(ServerUtils::Settings::UDP_REQUEST_TIMEOUT).GetInt(), sSingleton.getValue(ServerUtils::Settings::THREADS_NUMBER).GetInt());
	std::cout << "\nServer Started";

	server.startRoutines();
	std::cout << "\nSuccessfully started the server routines";

	server.accept();
	
	return 0;
}