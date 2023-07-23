#include <csignal>
#include <iostream>

#include "Log.hpp"

static void handleSignal(int a, siginfo_t *b, void *c)
{
	(void)b;
	(void)c;
	if (a == SIGTERM || a == SIGINT || a == SIGKILL)
	{
		Log::debug << "Recebido um sinal de parada;" << Log::eof;
	}
}

#ifndef TEST_MODE
int main(void)
{
	struct sigaction listenSignal;

	listenSignal.sa_sigaction = handleSignal;
	listenSignal.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &listenSignal, NULL);
	sigaction(SIGINT, &listenSignal, NULL);
	sigaction(SIGKILL, &listenSignal, NULL);

	Log::fatal << "Teste" << Log::eof;

	return (0);
}
#else
#include <gtest/gtest.h>
int main(int argc, char **argv)
{
	// Inicialize o Google Test
	::testing::InitGoogleTest(&argc, argv);

	// Execute os testes
	return RUN_ALL_TESTS();
	return (0);
}

static int sum(int a, int b)
{
	return a + b;
}

TEST(SumTest, DemonstrandoOsTestesUnitários)
{
	EXPECT_EQ(5, sum(3, 2)); // Sucesso
	EXPECT_EQ(5, sum(2, 2)); // Falha
}

#endif // TEST_MODE
