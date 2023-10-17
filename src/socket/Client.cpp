#include "socket/Client.hpp"

// STATIC LOCAL
// STATIC PUBLIC
// STATIC PROTECTED
// STATIC PRIVATE
// PUBLIC
Client::Client(int fd)
{
	this->pollfd.fd = fd;
	this->pollfd.revents = 0;
	this->pollfd.events = POLLIN | POLLOUT;
}
Client::~Client()
{
}
Error Client::init()
{
	int flags = -1;

	if ((flags = fcntl(this->pollfd.fd, F_GETFL, 0)) == ERROR)
		return makeError("fcntl get flags error: " + std::string(strerror(errno)));

	if (fcntl(this->pollfd.fd, F_SETFL, flags | O_NONBLOCK) == ERROR)
		return makeError("fcntl set flags error: " + std::string(strerror(errno)));

	return makeSuccess();
}
bool Client::isServer()
{
	return false;
}
// PROTECTED
// PRIVATE
