#include "unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_shouldFail(void)
{
	TEST_FAIL();
}

void test_shouldPass(void)
{
	TEST_PASS();
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_shouldFail);
	RUN_TEST(test_shouldPass);

	return UNITY_END();
}
