#include "../extern/unity/src/unity.h"
#include <string.h>
#include "communicationEndpoint.h"
#include "Broker.h"
#include "posting.h"
#include "protocol.h"

void setUp(void) {
}

void tearDown(void) {
}

char *ptr;

void func(Posting posting) {
    ptr = posting.data;
}

void test_publishSubscribeFromData(void) {
    Subscriber sub = (Subscriber) {.deliver=func};
    subscribeForData("test0", sub);
    subscribeForData("test1", sub);


    publishData("test0", "testData0");
    if (strcmp("testData0", ptr) != 0) {
        TEST_FAIL();
    }
    publishData("test1", "testData1");
    if (strcmp("testData1", ptr) != 0) {
        TEST_FAIL();
    }
}

void test_publishUnsubscribeFromData(void) {
    Subscriber sub = (Subscriber) {.deliver=func};

    subscribeForData("test0", sub);
    subscribeForData("test1", sub);

    publishData("test0", "testData0");
    if (strcmp("testData0", ptr) != 0) {
        TEST_FAIL();
    }
    publishData("test1", "testData1");
    if (strcmp("testData1", ptr) != 0) {
        TEST_FAIL();
    }
    unsubscribeFromData("test0", sub);
    publishData("test0", "testData0"); //Should not change the pointer as no longer subscribed
    if (strcmp("testData1", ptr) != 0) {
        TEST_FAIL();
    }
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_publishSubscribeFromData);
    RUN_TEST(test_publishUnsubscribeFromData);

    return UNITY_END();
}
