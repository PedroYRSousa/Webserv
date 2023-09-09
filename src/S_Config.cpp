#include "S_Config.hpp"

// STATIC LOCAL
// STATIC PUBLIC
int S_Config::readFile(std::string *readFileOut)
{
	S_Config config = S_Config::_instance;
	std::ifstream file((*readFileOut).c_str());

	if (file.fail())
	{
		(*readFileOut) = strerror(errno);
		return ERROR;
	}

	// Realiza a leitura do arquivo aqui

	return (0);
}
// STATIC PROTECTED
// STATIC PRIVATE
S_Config S_Config::_instance = S_Config();
S_Config &S_Config::getInstance(void)
{
	return S_Config::_instance;
}
// PUBLIC
// PROTECTED
// PRIVATE
S_Config::S_Config(void)
{
}
S_Config::~S_Config(void)
{
}
