#include "../extern/unity/src/unity.h"
#include <string.h>
#include "communicationEndpoint.h"
#include "posting.h"
#include "protocol.h"

char *lastDelivered;

void setUp(void) {
    lastDelivered = "\n";
}

void tearDown(void) {
    lastDelivered = "\n";
}

void checkLastData(char *expected) {
//    printf("EXPECTED:%s\n", expected);
//    printf("ACTUAL:%s\n", lastDelivered);
    TEST_ASSERT_EQUAL(expected, lastDelivered);
}

void deliver(Posting posting) {
    lastDelivered = posting.data;
}

void test_publishSubscribeForData(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForData("test0", sub);
    subscribeForData("test1", sub);

    publishData("test0", "testData0");
    checkLastData("testData0");

    publishData("test1", "testData1");
    checkLastData("testData1");

    tearDown();
}

void test_publishUnsubscribeFromData(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribeForData("test0", sub);
    subscribeForData("test1", sub);

    publishData("test0", "testData0");
    checkLastData("testData0");

    publishData("test1", "testData1");
    checkLastData("testData1");

    unsubscribeFromData("test0", sub);
    publishData("test0", "testData0");
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    checkLastData("testData1");

    tearDown();
}

void test_publishSubscribeForHeartbeat(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForHeartbeat("test0", sub);
    subscribeForHeartbeat("test1", sub);

    publishHeartbeat("test0");
    checkLastData("test0");
    publishHeartbeat("test1");
    checkLastData("test1");

    tearDown();
}

void test_publishUnsubscribeFromHeartbeat(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribeForHeartbeat("test0", sub);
    subscribeForHeartbeat("test1", sub);

    publishHeartbeat("test0");
    checkLastData("test0");
    publishHeartbeat("test1");
    checkLastData("test1");

    unsubscribeFromHeartbeat("test0", sub);
    publishHeartbeat("test0");
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    checkLastData("test1");

    tearDown();
}

void test_subscribeForDataStartRequest(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForDataStartRequest("test0", sub);
    subscribeForDataStartRequest("test1", sub);

    publishDataStartRequest("test0", "testData0");
    checkLastData("testData0");

    publishDataStartRequest("test1", "testData1");
    checkLastData("testData1");

    tearDown();
}

void test_subscribeForDataStopRequest(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForDataStopRequest("test0", sub);
    subscribeForDataStopRequest("test1", sub);

    publishDataStopRequest("test0", "testData0");
    checkLastData("testData0");

    publishDataStopRequest("test1", "testData1");
    checkLastData("testData1");

    tearDown();
}

void test_publishCommand(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("SET/test0", sub);
    subscribe("SET/test1", sub);

    publishCommand("test0", "0");
    checkLastData("0");

    publishCommand("test1", "1");
    checkLastData("1");

    tearDown();
}


void test_publishOnCommand(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("SET/test0", sub);

    publishOnCommand("test0");
    checkLastData("1");

    tearDown();
}

void test_publishOffCommand(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("SET/test0", sub);

    publishOffCommand("test0");
    checkLastData("0");

    tearDown();
}

void test_subscribeForLost(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForLost("test0", sub);
    subscribeForLost("test1", sub);


    publish((Posting) {.topic="test0/LOST", .data="testData0"});
    checkLastData("testData0");

    publish((Posting) {.topic="test1/LOST", .data="testData1"});
    checkLastData("testData1");

    tearDown();
}

void test_unsubscribeFromLost(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribeForLost("test0", sub);
    subscribeForLost("test1", sub);

    publish((Posting) {.topic="test0/LOST", .data="testData0"});
    checkLastData("testData0");

    publish((Posting) {.topic="test1/LOST", .data="testData1"});
    checkLastData("testData1");

    unsubscribeFromLost("test0", sub);
    publish((Posting) {.topic="test0/LOST", .data="testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    checkLastData("testData1");

    tearDown();
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_publishSubscribeForData);
    RUN_TEST(test_publishUnsubscribeFromData);

    RUN_TEST(test_publishSubscribeForHeartbeat);
    RUN_TEST(test_publishUnsubscribeFromHeartbeat);

    RUN_TEST(test_subscribeForDataStartRequest);
    RUN_TEST(test_subscribeForDataStopRequest);

    RUN_TEST(test_publishCommand);
    RUN_TEST(test_publishOnCommand);
    RUN_TEST(test_publishOffCommand);

    RUN_TEST(test_subscribeForLost);
    RUN_TEST(test_unsubscribeFromLost);

    return UNITY_END();
}