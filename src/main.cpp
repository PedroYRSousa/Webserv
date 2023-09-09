#include <cstring>
#include <csignal>
#include <iostream>
#include <cstdlib>

#include "Log.hpp"
#include "S_Config.hpp"

static void showHelp(void)
{
	std::cout << "Webserv :\
\n	Programa criado para como atividade da 42 São Paulo com base no ngix.\
\nInputs:\
\n	Nao ha necessidade de flag para passar o arquivo de configuracao.\
\nUso:\
\n	./webserv [arquivo de configuracao] (flags)\
\n	[] => obrigatorio	() => opcional\
\nFlags :\
\n	--debug -d: Mostra todos os logs.\
\n	--info -i: Mostra todos os logs do tipo info, warning, erro e fatal."
			  << std::endl;
	exit(0);
}

static void handleSignal(int a, siginfo_t *b, void *c)
{
	(void)b;
	(void)c;

	if (a == SIGTERM || a == SIGINT || a == SIGKILL)
	{
		Log::info << "Recebido sinal de parada" << Log::eof;
		Log::debug << "Sinal: " << a << Log::eof;
		exit(0);
	}
}

static int parseFlags(size_t argc, char **argv, std::string *parseFlagsOut)
{
	for (size_t i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0)
			Log::setLevelLog(DEBUG_LEVEL);
		else if (strcmp(argv[i], "--info") == 0 || strcmp(argv[i], "-i") == 0)
			Log::setLevelLog(INFO_LEVEL);
		else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
			showHelp();
		else
		{
			if (!(*parseFlagsOut).empty())
			{
				(*parseFlagsOut) = "Excesso de argumentos";
				return (ERROR);
			}

			(*parseFlagsOut) = argv[i];
		}
	}

	if ((*parseFlagsOut).empty())
	{
		(*parseFlagsOut) = "O arquivo de configuracao e obrigatorio";
		return (ERROR);
	}

	return (0);
}

#ifndef TEST_MODE // Normal
int main(int argc, char **argv)
{
	int err = 0;
	std::string parseFlagsOut = "";
	struct sigaction listenSignal;

	Log::setLevelLog(WARN_LEVEL);

	Log::info << "Iniciando o processo de captura de sinais." << Log::eof;
	listenSignal.sa_sigaction = handleSignal;
	listenSignal.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &listenSignal, NULL);
	sigaction(SIGINT, &listenSignal, NULL);
	sigaction(SIGKILL, &listenSignal, NULL);

	err = parseFlags(argc, argv, &parseFlagsOut);
	if (err == ERROR)
		Log::fatal << "Erro ao ler os argumentos: " << parseFlagsOut << Log::eof;

	err = S_Config::readFile(&parseFlagsOut);
	if (err == ERROR)
		Log::fatal << "Erro ao ler arquivo de configuracao: " << parseFlagsOut << Log::eof;

	Log::info << "Fim do programa." << Log::eof;
	return (0);
}
#else // Testes
#include <gtest/gtest.h>
int main(int argc, char **argv)
{
	// Inicialize o Google Test
	::testing::InitGoogleTest(&argc, argv);

	// Execute os testes
	return RUN_ALL_TESTS();
}

#endif // TEST_MODE
