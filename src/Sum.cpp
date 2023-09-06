#include "Sum.hpp"

int sum(int a, int b)
{
  return a + b;
}

#ifdef TEST_MODE
#include <gtest/gtest.h>

TEST(SumTest, DemonstrandoOsTestesUnitarios)
{
  EXPECT_EQ(5, sum(3, 2)); // Sucesso
  // EXPECT_EQ(5, sum(2, 2)); // Falha
}
#endif // TEST_MODE
