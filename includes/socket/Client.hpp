#ifndef CLIENT_HPP
#define CLIENT_HPP

// Importes com ""
#include "Log.hpp"
#include "Error.hpp"
#include "Socket.hpp"

// Importes com <>

// Defines
// #ifndef NAME
// #define NAME
// #endif // NAME

class Client : public Socket
{
public:
	Client(int fd);
	~Client();
	Error init();
	bool isServer();

protected:
private:
};

#endif // CLIENT_HPP
