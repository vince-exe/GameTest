#include <iostream>
	
#include "Server.h"
#include "ServerSettings.h"

int main() {
	ServerSettings& sSingleton = ServerSettings::getInstance();
	if (!sSingleton.init(SettingsFileUtils::SETTINGS_PATH)) {
		std::cerr << "\n [ ERROR ]: Failed to load the settings file";
		return 1;
	}

	unsigned int maxConnections;
	std::cout << "\nMax Connections: ";
	std::cin >> maxConnections;
	 
	unsigned int tcpPort, udpPort;
	std::cout << "\nTCP Port: ";
	std::cin >> tcpPort;

	std::cout << "\nUDP Port: ";
	std::cin >> udpPort;

	Server server(tcpPort, udpPort, maxConnections, sSingleton.getValue(SettingsFileUtils::CLEAR_USELESS_THREADS_EACH).GetInt());
	std::cout << "\nServer Started";

	server.startRoutines();
	std::cout << "\nSuccessfully started the server routines";

	server.accept();
	
	return 0;
}