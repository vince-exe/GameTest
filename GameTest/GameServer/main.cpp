#include <iostream>
	
#include "Server.h"

int main() {
	int maxConnections;

	std::cout << "\nMax Connections: ";
	std::cin >> maxConnections;
	
	Server server(8888, maxConnections);
	std::cout << "\nServer Started";

	server.startRoutines();
	std::cout << "\nSuccessfully started the server routines";

	server.accept();

	return 0;
}