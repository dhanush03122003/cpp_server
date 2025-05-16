#include <winsock2.h>
#include <iostream>
#include <string>
#include <thread>

#define SOCKET_BIND_ERROR -1

class SocketHandler
{
private:
	SOCKET sock_fd;
public:
	SocketHandler();
	~SocketHandler();

    bool create();
    bool bind(const std::string& ip, uint16_t port = 8080);
    bool listen(int backlog = 5);
    SocketHandler* accept(std::string& clientIp, uint16_t& clientPort);
    bool connect(const std::string& serverIp, uint16_t serverPort);

    int send(const char* data, size_t len);
    int recv(char* buffer, size_t len);
    void handle_client(SocketHandler* client);
    
    void close();

    //SOCKET   raw();  // expose raw socket if needed
};