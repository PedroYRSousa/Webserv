#include "S_Config.hpp"

// STATIC LOCAL
// STATIC PUBLIC
Error S_Config::readFile(const std::string &filePath)
{
	S_Config::getInstance();
	Log::info << "Iniciando leitura do arquivo de configuracoes" << Log::eof;
	Log::debug << "Arquivo: " << filePath << Log::eof;

	S_Config config = S_Config::_instance;
	std::ifstream file(filePath.c_str());

	if (file.fail())
		return makeError(strerror(errno));

	// Realiza a leitura do arquivo aqui
	// Servidor para servir um arquivo

	return makeSucces();
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
