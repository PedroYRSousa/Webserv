#include "socket/Server.hpp"

// STATIC LOCAL
// STATIC PUBLIC
// STATIC PROTECTED
// STATIC PRIVATE
// PUBLIC
Server::Server(int port)
{
	this->opt = 1;
	this->pollfd.events = POLLIN;
	this->pollfd.revents = 0;
	this->pollfd.fd = 0;
	this->port = port;
	this->addrlen = sizeof(this->address);
}
Server::~Server()
{
}
Error Server::init()
{
	Log::info << "Iniciando um servidor" << Log::eof;

	if ((this->pollfd.fd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
		return makeError("socket error: " + std::string(strerror(errno)));

	Log::debug << "Porta: " << this->port << " FD: " << this->pollfd.fd << Log::eof;

	if (setsockopt(this->pollfd.fd, SOL_SOCKET, (SO_REUSEADDR | SO_REUSEPORT), &opt, sizeof(opt)) <= ERROR)
		return makeError("setsockopt error: " + std::string(strerror(errno)));

	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(this->port);

	if (bind(this->pollfd.fd, (struct sockaddr *)&this->address, sizeof(this->address)) <= ERROR)
		return makeError("bind error: " + std::string(strerror(errno)));

	if (listen(this->pollfd.fd, 3) <= ERROR)
		return makeError("listen error: " + std::string(strerror(errno)));

	// while (true)
	//{
	//	int new_socket;
	//	char buffer[1024] = {0};
	//	std::string hello = "Hello from server";
	//	if ((new_socket = accept(this->pollfd.fd, (struct sockaddr *)&address,
	//							 (socklen_t *)&addrlen)) < 0)
	//	{
	//		perror("accept");
	//		exit(EXIT_FAILURE);
	//	}
	//	read(new_socket, buffer, 1024);
	//	printf("%s\n", buffer);
	//	send(new_socket, hello.c_str(), hello.size(), 0);
	//	printf("Hello message sent\n");

	//	// closing the connected socket
	//	close(new_socket);
	//}
	//// closing the listening socket
	// shutdown(this->pollfd.fd, SHUT_RDWR);

	// exit(1);

	return makeSuccess();
}
bool Server::isServer()
{
	return true;
}
struct sockaddr_in &Server::getAddress()
{
	return this->address;
}
struct sockaddr *Server::getSockAddress()
{
	return (struct sockaddr *)&(this->address);
}
socklen_t *Server::getSockLenAddress()
{
	return (socklen_t *)&(this->addrlen);
}
// PROTECTED
// PRIVATE
