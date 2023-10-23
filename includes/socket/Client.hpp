#ifndef CLIENT_HPP
#define CLIENT_HPP

// Importes com ""
#include "Log.hpp"
#include "Error.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "conn/Request.hpp"
#include "conn/Response.hpp"

// Importes com <>
#include <pthread.h>

// Defines
// #ifndef NAME
// #define NAME
// #endif // NAME

class Client : public Socket
{
public:
	Client(int fd, Server *s);
	~Client();
	Error init();
	bool isServer();
	Response *getResponse();
	Request *getRequest();
	void digestRequest();
	void readRequest(std::string reqRaw);
	bool getIsDigesting();

	// Mover para private
	Response *res;
	Request *req;
	pthread_mutex_t mutex;

protected:
private:
	Server *s;
	bool digesting;
};

#endif // CLIENT_HPP
