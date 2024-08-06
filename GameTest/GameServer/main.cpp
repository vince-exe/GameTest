#include <iostream>
	
#include "Server.h"
#include "utils.h"
#include "extern_vaiables.h"

int main() {
	if (!g_serverSettings.init(ServerUtils::Settings::SETTINGS_DIRECTORY / ServerUtils::Settings::SETTINGS_PATH)) {
		std::cerr << "\n [ ERROR ]: Failed to load the settings file";
		return 1;
	}

	Server server(g_serverSettings.getValue(ServerUtils::Settings::TCP_PORT).GetInt(),
			      g_serverSettings.getValue(ServerUtils::Settings::UDP_PORT).GetInt(),
		          g_serverSettings.getValue(ServerUtils::Settings::MAX_CONNECTIONS).GetInt(),
		          g_serverSettings.getValue(ServerUtils::Settings::CLEAR_USELESS_THREADS).GetInt(),
		          g_serverSettings.getValue(ServerUtils::Settings::UDP_REQUEST_TIMEOUT).GetInt(),
		          g_serverSettings.getValue(ServerUtils::Settings::THREADS_NUMBER).GetInt());
	std::cout << "\nServer Started";

	server.startRoutines();
	std::cout << "\nSuccessfully started the server routines";

	server.accept();
	
	return 0;
}