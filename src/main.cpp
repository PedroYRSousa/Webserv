#include <iostream>

#include "Log.hpp"

#ifndef TEST_MODE
int main(void)
{
	std::cout << "Olá mundo" << std::endl;
	Log::debug << "Olá Log" << Log::eof;

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
#endif // TEST_MODE
