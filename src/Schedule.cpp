#include "Schedule.hpp"

// STATIC LOCAL
// STATIC PUBLIC
Error Schedule::start(std::string filePathConfig)
{
	Log::info << "Iniciando o schedule" << Log::eof;

	parseConfig(filePathConfig);
	int index = 0;

	for (std::vector<S_Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		Server *s = new Server((*it), index);
		Error err = s->init();
		if (err.status == ERROR)
			Log::fatal << err.message << Log::eof;
		Schedule::addSocket(s);
		index++;
	}

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
		{
			if (std::string(strerror(errno)).compare("Interrupted system call") != 0)
				return makeError("poll error: " + std::string(strerror(errno)));
			else
				break;
		}

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
	Log::info << "Evento: \n"
			  << "\tLeitura (in):" << (poll->revents & POLLIN)
			  << "\n\tEscrita (out):" << (poll->revents & POLLOUT) << Log::eof;

	// Aceita uma conexão de cliente
	if (poll->revents & POLLIN) // Entrada (read)
	{
		return Schedule::acceptConnection(poll, server);
	}
	// Ainda não sei como lidar e como seria
	if (poll->revents & POLLOUT) // Saida (write)
	{
		Log::info << "POLLOUT requisição" << Log::eof;
	}
	if (poll->revents & POLLERR) // Erro (fd fechado para leitura)
	{
		Log::info << "Error requisição" << Log::eof;
	}
	if (poll->revents & POLLHUP) // fd fechado do outro lado (fim)
	{
		Log::info << "POLLHUP requisição" << Log::eof;
	}
	if (poll->revents & POLLNVAL) // fd fechado do outro lado (fim)
	{
		Log::info << "POLLNVAL requisição" << Log::eof;
	}

	return makeSuccess();
}
Error Schedule::handleClient(struct pollfd *poll, Client *client)
{
	Log::info << "Um cliente tem uma requisição" << Log::eof;
	Log::info << "Evento: \n"
			  << "\tLeitura (in):" << (poll->revents & POLLIN)
			  << "\n\tEscrita (out):" << (poll->revents & POLLOUT) << Log::eof;

	if (poll->revents & POLLIN) // Entrada (read)
	{
		return Schedule::readClient(poll, client);
	}
	if (poll->revents & POLLOUT) // Saida (write)
	{
		return Schedule::writeClient(poll, client);
	}
	if (poll->revents & POLLERR) // Erro (fd fechado para leitura)
	{
		Log::info << "Error requisição" << Log::eof;
	}
	if (poll->revents & POLLHUP) // fd fechado do outro lado (fim)
	{
		Log::info << "POLLHUP requisição" << Log::eof;
	}
	if (poll->revents & POLLNVAL) // fd fechado do outro lado (fim)
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

	Client *c = new Client(fd, server);
	c->init();
	Schedule::addSocket(c);

	return makeSuccess();
}
Error Schedule::readClient(struct pollfd *poll, Client *client)
{
	Log::info << "Lendo requisição" << Log::eof;

	char buffer[1048576] = {0}; // 1 Mega
	int valread = recv(poll->fd, buffer, sizeof(buffer), 0);

	if (valread == 0) // FD Fechado pelo cliente
	{
		Schedule::removeSocket(client);
	}
	else if (valread <= ERROR) // Lidando com erro de leitura
	{
		client->digestRequest();
	}
	else
		client->readRequest(std::string(buffer, valread));

	Log::debug << buffer << Log::eof;

	return makeSuccess();
}
Error Schedule::writeClient(struct pollfd *poll, Client *client)
{
	if (client->getResponse() == NULL && !client->getIsDigesting())
	{
		Log::info << "Processando requisição" << Log::eof;

		client->digestRequest();
	}
	else if (client->getResponse() != NULL)
	{
		Log::info << "Enviando resposta" << Log::eof;

		S_Response *res = client->getResponse();
		std::string resDumped = generateOutMessage(*res);
		Log::debug << resDumped.c_str() << Log::eof;

		int valsend = send(poll->fd, resDumped.c_str(), resDumped.size(), 0);
		if (valsend == 0)
			Log::error << "FD Ja fechado pelo cliente" << Log::eof;
		if (valsend <= ERROR)
			Log::error << "Erro ao enviar alguma resposta" << Log::eof;

		Schedule::removeSocket(client);
	}

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
