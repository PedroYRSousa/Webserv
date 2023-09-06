#include <csignal>
#include <iostream>
#include <cstdlib>

#include "Log.hpp"
#include "Sum.hpp"

static void handleSignal(int a, siginfo_t *b, void *c)
{
	(void)b;
	(void)c;
	if (a == SIGTERM || a == SIGINT || a == SIGKILL)
	{
		exit(0);
	}
}

#ifndef TEST_MODE // Normal
int main(void)
{
	struct sigaction listenSignal;

	Log::setLevelLog(DEBUG_LEVEL);

	std::cout << Log::getLevelLog() << std::endl;
	std::cout << Log::getLevelLog() << std::endl;

	listenSignal.sa_sigaction = handleSignal;
	listenSignal.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &listenSignal, NULL);
	sigaction(SIGINT, &listenSignal, NULL);
	sigaction(SIGKILL, &listenSignal, NULL);

	while (true)
	{
		Log::fatal << "Teste" << Log::eof;
	}

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
