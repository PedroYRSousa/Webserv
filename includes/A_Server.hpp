#ifndef A_SERVER_HPP
#define A_SERVER_HPP

#include "A_Connection.hpp"

#include <iostream>

class A_Server : public A_Connection
{
public:
protected:
	A_Server(int port);
	~A_Server();

	Error init(void);

private:
	const int port;
};

#endif // A_SERVER_HPP
