#include <cstring>
#include <csignal>
#include <iostream>
#include <cstdlib>

#include "S_Schedule.hpp"
#include "Log.hpp"

static void handleSignal(int a, siginfo_t *b, void *c)
{
	(void)b;
	(void)c;
	if (a == SIGTERM || a == SIGINT || a == SIGKILL)
	{
		exit(0);
	}
}

static void parseFlags(size_t argc, char **argv)
{
	if (argc < 2)
		return;

	for (size_t i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0)
			Log::setLevelLog(DEBUG_LEVEL);
		if (strcmp(argv[i], "--info") == 0 || strcmp(argv[i], "-i") == 0)
			Log::setLevelLog(INFO_LEVEL);
		if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
		{
			std::cout << "Webserv:\n\tPrograma criado para como atividade da 42 São Paulo com base no ngix.\nFlags:\n\t--debug -d: Mostra todos os logs.\n\t--info -i: Mostra todos os logs do tipo info, warning, erro e fatal." << std::endl;
			exit(0);
		}
	}
}

#ifndef TEST_MODE // Normal
int main(int argc, char **argv)
{
	struct sigaction listenSignal;

	Log::setLevelLog(WARN_LEVEL);

	parseFlags(argc, argv);

	listenSignal.sa_sigaction = handleSignal;
	listenSignal.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &listenSignal, NULL);
	sigaction(SIGINT, &listenSignal, NULL);
	sigaction(SIGKILL, &listenSignal, NULL);

	S_Schedule::start(argc, argv);
	S_Schedule::loop();
	S_Schedule::end();
	S_Schedule::stop();

	S_Schedule::getInstance();

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
