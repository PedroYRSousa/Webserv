#ifndef A_CONNECTION_HPP
#define A_CONNECTION_HPP

#include "Error.hpp"

#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

class A_Connection
{
public:
	pollfd &getPoll();
	virtual Error init(void) = 0;

protected:
	static std::vector<pollfd *> polls;
	pollfd poll;

	A_Connection();
	~A_Connection();

private:
};

#endif // A_CONNECTION_HPP
