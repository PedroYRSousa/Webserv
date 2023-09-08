#include "Log.hpp"
#include "S_Schedule.hpp"

// STATIC PUBLIC
S_Schedule &S_Schedule::getInstance(void)
{
	return S_Schedule::_instance;
}
void S_Schedule::start(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	S_Schedule::getInstance();

	// Ler o arquivo de configuração
	// O que tem aqui é temporario ate a criação do arquivo e do parser
	//	// Obtenção de pagina  html
	_instance.addServer(8000, Server());
}
void S_Schedule::loop(void)
{
	while (_instance.toContinue)
	{
	}
}
void S_Schedule::end(void)
{
}
void S_Schedule::stop(void)
{
	_instance.toContinue = false;
	_instance.servers.clear();
}

// STATIC PROTECTED

// STATIC PRIVATE
S_Schedule S_Schedule::_instance = S_Schedule();

// PUBLIC

// PROTECTED

// PRIVATE
S_Schedule::S_Schedule(void)
{
	this->toContinue = true;
	this->servers = std::map<int, Server>();
}
S_Schedule::~S_Schedule(void)
{
}
size_t S_Schedule::addServer(int port, Server server)
{
	this->servers.insert(std::pair<int, Server>(port, server));

	return this->servers.size();
}
