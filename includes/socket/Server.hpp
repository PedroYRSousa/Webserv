#ifndef SERVER_HPP
#define SERVER_HPP

// Importes com ""
#include "Log.hpp"
#include "Error.hpp"
#include "Socket.hpp"

// Importes com <>
#include "map"
#include "vector"

// Defines
// #ifndef NAME
// #define NAME
// #endif // NAME

class Server : public Socket
{
public:
	Server(int port);
	~Server();
	Error init();
	bool isServer();
	struct sockaddr_in &getAddress();
	struct sockaddr *getSockAddress();
	socklen_t *getSockLenAddress();

protected:
private:
	int opt;
	int port;
	int addrlen;
	struct sockaddr_in address;
};

#endif // SERVER_HPP
