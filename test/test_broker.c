#include "../extern/unity/src/unity.h"
#include <string.h>
#include "posting.h"
#include "subscriber.h"
#include "communicationEndpoint.h"

char *lastDelivered;

void setUp(void) {
    lastDelivered = "\n";
}

void tearDown(void) {
    lastDelivered = "\n";
}

void checkLastData(char *expected) {
    if (strcmp(expected, lastDelivered) != 0) {
        printf("EXPECTED:%s\n", expected);
        printf("ACTUAL:%s\n", lastDelivered);
        TEST_FAIL();
    }
}

void deliver(Posting posting) {
    lastDelivered = posting.data;
}

void test_publishSubscribeRaw(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("test0", sub);
    subscribe("test1", sub);

    publish((Posting) {.topic="test0", .data="testData0"});
    checkLastData("testData0");

    publish((Posting) {.topic="test1", .data="testData1"});
    checkLastData("testData1");

    tearDown();
}

void test_publishUnsubscribeRaw(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("test0", sub);
    subscribe("test1", sub);

    publish((Posting) {.topic="test0", .data="testData0"});
    checkLastData("testData0");

    publish((Posting) {.topic="test1", .data="testData1"});
    checkLastData("testData1");

    unsubscribe("test0", sub);
    publish((Posting) {.topic="test0", .data="testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    checkLastData("testData1");

    tearDown();
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_publishSubscribeRaw);
    RUN_TEST(test_publishUnsubscribeRaw);

    return UNITY_END();
}
