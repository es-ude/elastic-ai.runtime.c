#include "../extern/unity/src/unity.h"
#include <string.h>
#include "Broker.h"

void setUp(void) {
}

void tearDown(void) {
}

void func(char *topic, uint32_t topic_length, uint32_t payload) {
    printf("%s\n", topic);

    TEST_PASS();
}

void test_publishSubscribe(void) {
    char topic[26] = "eip://domain/DATA/sensor/1";
    uint32_t topic_length = strlen(topic);
    Subscription_t newSubscription = (Subscription_t) {.topic=*topic, .callback_fct =&func};
    subscribe(newSubscription);
    publish(topic, topic_length, 4);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_publishSubscribe);

    return UNITY_END();
}
