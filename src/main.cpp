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

#ifndef TEST_MODE // Normal
int main(int argc, char **argv)
{
	struct sigaction listenSignal;

	listenSignal.sa_sigaction = handleSignal;
	listenSignal.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &listenSignal, NULL);
	sigaction(SIGINT, &listenSignal, NULL);
	sigaction(SIGKILL, &listenSignal, NULL);

	Log::setLevelLog(DEBUG_LEVEL);
	Log::getLevelLog();

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
