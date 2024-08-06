#pragma once

#include <iostream>
#include <filesystem>

namespace ServerUtils {
	namespace Settings {
		const std::string MAX_CONNECTIONS = "Max-Connections";

		const std::string TCP_PORT = "TCP-Port";

		const std::string UDP_PORT = "UDP-Port";

		const std::string CLEAR_USELESS_THREADS = "Clear-Useless-Threads-Each";

		const std::string UDP_REQUEST_TIMEOUT = "UDP-Request-Timeout";

		const std::string THREADS_NUMBER = "ThreadsNumber";

		const std::filesystem::path SETTINGS_DIRECTORY = "settings";

		const std::filesystem::path SETTINGS_PATH = "settings.json";
	}
}