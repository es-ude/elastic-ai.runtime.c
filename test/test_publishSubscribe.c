#include "unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}


void test_publishSubscribe(void){
    TEST_FAIL();
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_publishSubscribe);

    return UNITY_END();
}