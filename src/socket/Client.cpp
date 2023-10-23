#include "socket/Client.hpp"

// STATIC LOCAL
// STATIC PUBLIC
// STATIC PROTECTED
// STATIC PRIVATE
// PUBLIC
Client::Client(int fd, Server *s)
{
	pthread_mutex_init(&mutex, NULL);
	this->digesting = false;
	this->s = s;
	this->pollfd.fd = fd;
	this->pollfd.revents = 0;
	this->pollfd.events = POLLIN | POLLOUT;
	this->req = NULL;
	this->res = NULL;
}
Client::~Client()
{
	pthread_mutex_destroy(&mutex);
	if (this->req != NULL)
		delete this->req;
	if (this->res != NULL)
		delete this->res;
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
Response *Client::getResponse()
{
	return this->res;
}
Request *Client::getRequest()
{
	return this->req;
}
void Client::digestRequest()
{
	this->digesting = true;

	if (this->res)
		delete this->res;

	if (this->req == NULL)
	{
		this->res = new Response();
		this->res->setStatus(500);
	}
	else
	{
		this->res = new Response();
		this->res->setStatus(201);
	}
}
void Client::readRequest(std::string reqRaw)
{
	if (this->req == NULL)
	{
		this->req = new Request();
		Request::parser(reqRaw, this->req);
	}
	else
		this->req->addInBody(reqRaw);
}
bool Client::getIsDigesting()
{
	return this->digesting;
}
// PROTECTED
// PRIVATE
