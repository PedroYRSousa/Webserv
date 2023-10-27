#include "socket/Client.hpp"

// STATIC LOCAL
static int getMethodIndex(std::string method)
{
	if (method.compare("GET") == 0)
		return GET;
	if (method.compare("POST") == 0)
		return POST;
	if (method.compare("DELETE") == 0)
		return DELETE;

	return ERROR;
}
// STATIC PUBLIC
// STATIC PROTECTED
// STATIC PRIVATE
// PUBLIC
Client::Client(int fd, Server *s)
{
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
S_Response *Client::getResponse()
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
		this->res = new S_Response();
		this->res->status_code = 500;
	}
	else
	{
		S_Server serverStruct = this->s->getStruct();
		std::string host;

		Error err = this->req->getHeader("host", &host);
		if (err.status == ERROR)
		{
			this->res = new S_Response();
			this->res->status_code = 400;
			return;
		}

		host = std::string(host, 0, host.find(":"));

		if (serverStruct.server_name.size() <= 0)
		{
			this->resolveRequest();
			// Vai resolver ou negar?
		}
		for (std::vector<std::string>::iterator it = serverStruct.server_name.begin(); it != serverStruct.server_name.end(); it++)
		{
			if ((*it).compare(host) == 0)
			{
				this->resolveRequest();
				break;
			}
		}
		if (this->res == NULL)
		{
			this->res = new S_Response();
			this->res->status_code = 404;
		}
	}
}
void Client::readRequest(std::string reqRaw)
{
	std::string rec = "\r";
	size_t found = reqRaw.find(rec);
	while (found != std::string::npos)
	{
		reqRaw.replace(found, rec.length(), "");
		found = reqRaw.find(rec);
	}

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
void Client::resolveRequest()
{
	S_Request reqStruct;
	int index = this->s->getIndexServerStruct();

	reqStruct.server_number = index;
	reqStruct.body = this->req->getBody();
	reqStruct.header_fields = this->req->getAllHeaders();
	reqStruct.method = getMethodIndex(this->req->getMethod());
	if (reqStruct.method == ERROR)
	{
		this->res = new S_Response();
		this->res->status_code = 405;
		return;
	}
	reqStruct.path = this->req->getURI();
	std::cout << reqStruct.path << std::endl;
	reqStruct.queryString = this->req->getQueryString();

	S_Response resStruct = run(reqStruct);

	this->res = new S_Response();
	this->res->body = resStruct.body;
	this->res->header_fields = resStruct.header_fields;
	this->res->server_number = resStruct.server_number;
	this->res->status_code = resStruct.status_code;
}
