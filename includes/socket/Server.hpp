#ifndef SERVER_HPP
#define SERVER_HPP

// Importes com ""
#include "lib.hpp"
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
	Server(S_Server s);
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
	S_Server serverStruct;
};

#endif // SERVER_HPP
