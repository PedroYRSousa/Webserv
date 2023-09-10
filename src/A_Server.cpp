#include "A_Server.hpp"

// STATIC LOCAL
// STATIC PUBLIC
// STATIC PROTECTED
// STATIC PRIVATE
// PUBLIC
Error A_Server::init(void)
{
	//	POLLIN
	//		There is data to read.
	//	POLLOUT
	//		Writing is now possible, though a write larger than the
	//		available space in a socket or pipe will still block
	//		(unless O_NONBLOCK is set).
	//	POLLERR
	//		Error condition (only returned in revents; ignored in
	//		events).  This bit is also set for a file descriptor
	//		referring to the write end of a pipe when the read end has
	//		been closed.
	//	POLLHUP
	//		Hang up (only returned in revents; ignored in events).
	//		Note that when reading from a channel such as a pipe or a
	//		stream socket, this event merely indicates that the peer
	//		closed its end of the channel.  Subsequent reads from the
	//		channel will return 0 (end of file) only after all
	//		outstanding data in the channel has been consumed.
	//	POLLNVAL
	//		Invalid request: fd not open (only returned in revents;
	//		ignored in events).
	this->poll.events = (POLLIN | POLLOUT);
	this->poll.revents = 0;

	this->poll.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->poll.fd == ERROR)
		return makeError(strerror(errno));

	if (setsockopt(this->poll.fd, SOL_SOCKET, (SO_REUSEADDR | SO_REUSEPORT), &this->opt, sizeof(this->opt)) == ERROR)
		return makeError(strerror(errno));

	this->addr.sin_addr.s_addr = INADDR_ANY;
	this->addr.sin_family = AF_INET;
	this->addr.sin_port = htons(this->port);

	if (bind(this->poll.fd, (struct sockaddr *)&this->addr, sizeof(this->addr)) == ERROR)
		return makeError(strerror(errno));

	if (listen(this->poll.fd, -1) == ERROR)
		return makeError(strerror(errno));
}
// PROTECTED
A_Server::A_Server(int port) : port(port)
{
}
A_Server::~A_Server()
{
}
// PRIVATE
