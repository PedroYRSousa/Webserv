#ifndef A_SERVER_HPP
#define A_SERVER_HPP

#include "A_Connection.hpp"

#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

class A_Server : public A_Connection
{
public:
	Error init(void);

protected:
	A_Server(int port);
	~A_Server();

private:
	int opt;
	sockaddr_in addr;
	const int port;
};

#endif // A_SERVER_HPP
