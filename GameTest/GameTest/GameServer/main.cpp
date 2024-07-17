#include <iostream>
	
#include "Server.h"
#include "ServerSettings.h"

int main() {
	ServerSettings& sSingleton = ServerSettings::getInstance();
	if (!sSingleton.init(SettingsFileUtils::SETTINGS_PATH)) {
		std::cerr << "\n [ ERROR ]: Failed to load the settings file\n";
		system("pause");
		return 1;
	}

	unsigned int maxConnections;
	std::cout << "\nMax Connections: ";
	std::cin >> maxConnections;
	 
	std::string ipAddress;
	std::cout << "\nIp Address: ";
	std::cin >> ipAddress;

	unsigned int port;
	std::cout << "\nServer Port: ";
	std::cin >> port;

	Server server(port, ipAddress, maxConnections, sSingleton.getValue(SettingsFileUtils::CLEAR_USELESS_THREADS_EACH).GetInt());
	std::cout << "\nServer Started";

	server.startRoutines();
	std::cout << "\nSuccessfully started the server routines";

	server.accept();

	return 0;
}