#include "SocketHandler.hpp"
#include "HttpsClient.hpp"

int SocketHandler::winsock_init_count = 0;

SocketHandler::SocketHandler() : sock_fd(-1) {
#ifdef _WIN32
    if (winsock_init_count++ == 0) {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            std::cerr << "WSAStartup failed\n";
        }
    }
#endif
}

SocketHandler::~SocketHandler() {
#ifdef _WIN32
    if (--winsock_init_count == 0) {
        WSACleanup();
    }
#endif
}

bool 
SocketHandler::create() {
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error code: " << WSAGetLastError() << std::endl;
        return false;
    }
	return true;
}

bool 
SocketHandler::bind(const std::string& ip, uint16_t port) {
    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    return :: bind(sock_fd, (struct sockaddr*)&serverAddress,
        sizeof(serverAddress)) != SOCKET_BIND_ERROR;;
}

bool 
SocketHandler::listen(int backlog) {
    return ::listen(sock_fd, backlog) != SOCKET_ERROR;
}

SocketHandler* 
SocketHandler::accept(std::string& clientIp, uint16_t& clientPort) {
    sockaddr_in clientAddr{};
    int addrLen = sizeof(clientAddr);

    // Accept a connection request from a client
    SOCKET clientSock = ::accept(sock_fd, (sockaddr*)&clientAddr, &addrLen);//sock_fd , null , null
    if (clientSock == INVALID_SOCKET) return nullptr;

    SocketHandler* client = new SocketHandler();
    client->sock_fd = clientSock;
    return client;
}

bool
SocketHandler::connect(const std::string& serverIp, uint16_t serverPort) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serverPort);

    if (InetPton(AF_INET, serverIp.c_str(), &addr.sin_addr) != 1) {
        // Failed to convert IP address
        return false;
    }

    return ::connect(sock_fd, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR;
}


int 
SocketHandler :: send(const char* data, size_t len) {
    return ::send(sock_fd, data, static_cast<int>(len), 0);
}
//
int 
SocketHandler:: recv(char* buffer, size_t len) {
    return ::recv(sock_fd, buffer, static_cast<int>(len), 0);
}

void 
SocketHandler :: close() {
#ifdef _WIN32
    ::closesocket(sock_fd);
#else
    ::shutdown(sock_, SHUT_RDWR);
    ::close(sock_);
#endif
    //is_valid_ = false;
}
void
SocketHandler::handle_client(SocketHandler* client) {
    char buf[4096];
    int bytes = client->recv(buf, sizeof(buf));

    //std::string message = "Hii cpp server !";

    //std::string response =
    //    "HTTP/1.1 200 OK\r\n"
    //    "Content-Type: text/plain\r\n"
    //    "Content-Length: " + std::to_string(message.length()) + "\r\n"
    //    "Connection: keep-alive\r\n"
    //    "\r\n" +
    //    message;


    //if (bytes > 0 && std::string(buf, bytes).find("GET /favicon.ico") == std::string::npos)
    if (bytes > 0)
    {
        HttpsClient httpclient = HttpsClient(std::string(buf, bytes));
        std::string response = httpclient.process_request();
        //std::cout << "inside bytes" << std::endl;
        //std::cout << "Received: " << std::string(buf, bytes) << "\n";
        client->send(response.c_str(), response.length());
    }

    client->close();
    delete client;

}
// SOCKET Socket::raw() { return sock_; }