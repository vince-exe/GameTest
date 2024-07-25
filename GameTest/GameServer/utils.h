#pragma once

#include <iostream>

namespace ServerUtils {
	namespace Settings {
		const std::string MAX_CONNECTIONS = "Max-Connections";

		const std::string TCP_PORT = "TCP-Port";

		const std::string UDP_PORT = "UDP-Port";

		const std::string CLEAR_USELESS_THREADS = "Clear-Useless-Threads-Each";

		const std::string SETTINGS_PATH = "settings/settings.json";
	}
}