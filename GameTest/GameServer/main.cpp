#include <iostream>
	
#include "Server.h"

int main() {
	int maxConnections;

	std::cout << "\nMax Connections: ";
	std::cin >> maxConnections;
	
	Server server(8888, maxConnections);

	std::cout << "\nServer Started\n";
	server.accept();

	return 0;
}