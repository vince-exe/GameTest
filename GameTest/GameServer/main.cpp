#include <iostream>
	
#include "Server.h"

int main() {
	Server server(8888);

	std::cout << "\nServer Avviato\n";
	server.accept();

	return 0;
}