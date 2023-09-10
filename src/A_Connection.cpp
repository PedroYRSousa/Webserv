#include "A_Connection.hpp"

// STATIC LOCAL
// STATIC PUBLIC
// STATIC PROTECTED
// STATIC PRIVATE
// PUBLIC
pollfd &A_Connection::getPoll()
{
	return this->poll;
}
// PROTECTED
A_Connection::A_Connection() : poll(pollfd())
{
}
A_Connection::~A_Connection()
{
	close(this->poll.fd);
	this->poll.events = 0;
	this->poll.revents = 0;
}
// PRIVATE
