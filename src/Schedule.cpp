#include "Schedule.hpp"

// STATIC LOCAL
// STATIC PUBLIC
Error Schedule::start(std::string filePathConfig)
{
	Log::info << "Iniciando o schedule" << Log::eof;

	// Realiza o parser do arquivo de configurações
	(void)filePathConfig;

	Server *s = new Server(8080);
	Error err = s->init();
	if (err.status == ERROR)
		Log::fatal << err.message << Log::eof;
	Schedule::addSocket(s);

	return makeSuccess();
}
Error Schedule::loop(void)
{
	Log::info << "Loop do schedule" << Log::eof;

	Error err;
	int result = 0;
	Socket *socket;
	struct pollfd *pollFD;

	while (Schedule::_instance.toContinue)
	{
		if ((result = poll(Schedule::_instance.polls.data(), Schedule::_instance.polls.size(), Schedule::_instance.timeout)) == ERROR)
			return makeError("poll error: " + std::string(strerror(errno)));

		Log::info << "Verificando os status das conexões: " << result << Log::eof;

		for (size_t i = 0; i < Schedule::_instance.polls.size(); i++)
		{
			pollFD = &Schedule::_instance.polls[i];
			socket = Schedule::_instance.sockets[pollFD->fd];

			if (pollFD->revents != 0 && socket->isServer())
			{
				err = Schedule::handleServer(pollFD, dynamic_cast<Server *>(socket));
				if (err.status == ERROR)
					return err;
			}
			else if (pollFD->revents != 0)
			{
				err = Schedule::handleClient(pollFD, dynamic_cast<Client *>(socket));
				if (err.status == ERROR)
					Log::error << err.message << Log::eof;
			}
		}
	}

	return makeSuccess();
}
Error Schedule::end(void)
{
	Log::info << "Finalizando o schedule" << Log::eof;

	return makeSuccess();
}
void Schedule::stop(void)
{
	Log::info << "Parando schedule" << Log::eof;

	Schedule::_instance.toContinue = false;
}

void Schedule::addSocket(Socket *s)
{
	Log::info << "Adicionando socket" << Log::eof;

	Schedule::_instance.sockets.insert(std::make_pair(s->getPollFD().fd, s));
	Schedule::_instance.polls.push_back(s->getPollFD());
}
void Schedule::removeSocket(Socket *s)
{
	Log::info << "Removendo socket" << Log::eof;

	Schedule::_instance.sockets.erase(s->getPollFD().fd);
	for (size_t i = 0; i < Schedule::_instance.polls.size(); i++)
	{
		if (Schedule::_instance.polls[i].fd == s->getPollFD().fd)
		{
			Schedule::_instance.polls.erase(Schedule::_instance.polls.begin() + i);
			break;
		}
	}

	delete s;
}
// STATIC PROTECTED
// STATIC PRIVATE
Schedule Schedule::_instance = Schedule();
Error Schedule::handleServer(struct pollfd *poll, Server *server)
{
	Log::info << "Um servidor tem uma requisição" << Log::eof;
	Log::info << poll->revents << Log::eof;

	int revents = poll->revents;
	poll->revents = 0;

	// Aceita uma conexão de cliente
	if (revents & POLLIN) // Entrada (read)
	{
		return Schedule::acceptConnection(poll, server);
	}
	// Ainda não sei como lidar e como seria
	if (revents & POLLOUT) // Saida (write)
	{
		Log::info << "POLLOUT requisição" << Log::eof;
	}
	if (revents & POLLERR) // Erro (fd fechado para leitura)
	{
		Log::info << "Error requisição" << Log::eof;
	}
	if (revents & POLLHUP) // fd fechado do outro lado (fim)
	{
		Log::info << "POLLHUP requisição" << Log::eof;
	}
	if (revents & POLLNVAL) // fd fechado do outro lado (fim)
	{
		Log::info << "POLLNVAL requisição" << Log::eof;
	}

	return makeSuccess();
}
Error Schedule::handleClient(struct pollfd *poll, Client *client)
{
	Log::info << "Um cliente tem uma requisição" << Log::eof;
	Log::info << poll->revents << Log::eof;

	int revents = poll->revents;
	poll->revents = 0;

	if (revents & POLLIN) // Entrada (read)
	{
		return Schedule::readClient(poll, client);
	}
	if (revents & POLLOUT) // Saida (write)
	{
		return Schedule::writeClient(poll, client);
	}
	if (revents & POLLERR) // Erro (fd fechado para leitura)
	{
		Log::info << "Error requisição" << Log::eof;
	}
	if (revents & POLLHUP) // fd fechado do outro lado (fim)
	{
		Log::info << "POLLHUP requisição" << Log::eof;
	}
	if (revents & POLLNVAL) // fd fechado do outro lado (fim)
	{
		Log::info << "POLLNVAL requisição" << Log::eof;
	}

	return makeSuccess();
}
Error Schedule::acceptConnection(struct pollfd *poll, Server *server)
{
	int fd = 0;

	if ((fd = accept(poll->fd, server->getSockAddress(), server->getSockLenAddress())) < 0)
		return makeError("accept error: " + std::string(strerror(errno)));

	Client *c = new Client(fd);
	c->init();
	Schedule::addSocket(c);

	return makeSuccess();
}
Error Schedule::readClient(struct pollfd *poll, Client *client)
{
	Log::info << "Lendo requisição" << Log::eof;

	char buffer[1024] = {0};
	int valread = recv(poll->fd, buffer, sizeof(buffer), 0);
	Log::info << valread << Log::eof;
	Log::info << buffer << Log::eof;

	if (valread == 0)
		Schedule::removeSocket(client);

	return makeSuccess();
}
Error Schedule::writeClient(struct pollfd *poll, Client *client)
{
	(void)client;
	Log::info << "Enviando resposta" << Log::eof;

	std::string httpVersion = "HTTP/1.1";
	std::string statusCode = "200 OK";
	std::string contentType = "text/html";
	std::string content = "<html><body><h1>Hello, World!</h1></body></html>";

	std::stringstream response;
	response << httpVersion << " " << statusCode << "\r\n";
	response << "Content-Type: " << contentType << "\r\n";
	response << "Content-Length: " << content.length() << "\r\n";
	response << "\r\n";
	response << content;
	std::string httpResponse = response.str();

	send(poll->fd, httpResponse.c_str(), httpResponse.size(), 0);

	Schedule::removeSocket(client);

	return makeSuccess();
}
// PUBLIC
// PROTECTED
// PRIVATE
Schedule::Schedule()
{
	this->timeout = -1;
	this->toContinue = true;
	this->sockets = std::map<int, Socket *>();
	this->polls = std::vector<struct pollfd>();
}
Schedule::~Schedule()
{
	for (std::map<int, Socket *>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}

	this->sockets.clear();
}
