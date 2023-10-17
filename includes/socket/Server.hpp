#ifndef SERVER_HPP
#define SERVER_HPP

// Importes com ""
#include "Log.hpp"
#include "Error.hpp"
#include "Socket.hpp"

// Importes com <>

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

	virtual void handleRequest() = 0;

protected:
private:
	int opt;
	int port;
	int addrlen;
	struct sockaddr_in address;
};

#endif // SERVER_HPP
