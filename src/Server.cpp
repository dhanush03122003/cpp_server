#include "Server.hpp"
#include "SocketHandler.hpp"

Server::Server()
{

}

Server::~Server()
{

}

void 
Server::start() {
SocketHandler socketHandler;

if (!socketHandler.create() || !socketHandler.bind("0.0.0.0") || !socketHandler.listen()) {
	std::cout << "server failed" << std::endl;
}
else {

	// exit();
}

std::string clientIp;
uint16_t clientPort;
while (1) {
	SocketHandler* client = socketHandler.accept(clientIp, clientPort);
	std::thread([client]() {
		client->handle_client(client);
		}).detach();
	
	//client->handle_client(client);
}

socketHandler.close();
}