#ifndef A_CONNECTION_HPP
#define A_CONNECTION_HPP

#include "Error.hpp"

#include <vector>
#include <poll.h>

class A_Connection
{
public:
	pollfd &getPoll();
	virtual Error init(void) = 0;

protected:
	static std::vector<pollfd *> polls;

private:
	pollfd poll;
};

#endif // A_CONNECTION_HPP
