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
    if (strcmp(expected, lastDelivered) != 0) {
        printf("EXPECTED:%s\n", expected);
        printf("ACTUAL:%s\n", lastDelivered);
        TEST_FAIL();
    }
}

void deliver(Posting posting) {
    lastDelivered = posting.data;
}

void test_publishSubscribeForData(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForData("testSubData0", sub);
    subscribeForData("testSubData1", sub);

    publishData("testSubData0", "testData0");
    checkLastData("testData0");

    publishData("testSubData1", "testData1");
    checkLastData("testData1");

    tearDown();
}

void test_publishUnsubscribeFromData(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribeForData("testUnsubData0", sub);
    subscribeForData("testUnsubData1", sub);

    publishData("testUnsubData0", "testData0");
    checkLastData("testData0");

    publishData("testUnsubData1", "testData1");
    checkLastData("testData1");

    unsubscribeFromData("testUnsubData0", sub);
    publishData("testUnsubData0", "testData0");
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    checkLastData("testData1");

    tearDown();
}

void test_publishSubscribeForHeartbeat(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForHeartbeat("testSubHeart0", sub);
    subscribeForHeartbeat("testSubHeart1", sub);

    publishHeartbeat("testSubHeart0");
    checkLastData("testSubHeart0");
    publishHeartbeat("testSubHeart1");
    checkLastData("testSubHeart1");

    tearDown();
}

void test_publishUnsubscribeFromHeartbeat(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribeForHeartbeat("testUnsubHeart0", sub);
    subscribeForHeartbeat("testUnsubHeart1", sub);

    publishHeartbeat("testUnsubHeart0");
    checkLastData("testUnsubHeart0");
    publishHeartbeat("testUnsubHeart1");
    checkLastData("testUnsubHeart1");

    unsubscribeFromHeartbeat("testUnsubHeart0", sub);
    publishHeartbeat("testUnsubHeart0");
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    checkLastData("testUnsubHeart1");

    tearDown();
}

void test_subscribeForDataStartRequest(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForDataStartRequest("testSubDataStart0", sub);
    subscribeForDataStartRequest("testSubDataStart1", sub);

    publishDataStartRequest("testSubDataStart0", "testData0");
    checkLastData("testData0");

    publishDataStartRequest("testSubDataStart1", "testData1");
    checkLastData("testData1");

    tearDown();
}

void test_subscribeForDataStopRequest(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForDataStopRequest("testSubDataStop0", sub);
    subscribeForDataStopRequest("testSubDataStop1", sub);

    publishDataStopRequest("testSubDataStop0", "testData0");
    checkLastData("testData0");

    publishDataStopRequest("testSubDataStop1", "testData1");
    checkLastData("testData1");

    tearDown();
}

void test_publishCommand(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("SET/testPubCmd0", sub);
    subscribe("SET/testPubCmd1", sub);

    publishCommand("testPubCmd0", "0");
    checkLastData("0");

    publishCommand("testPubCmd1", "1");
    checkLastData("1");

    tearDown();
}


void test_publishOnCommand(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("SET/testPubOn0", sub);

    publishOnCommand("testPubOn0");
    checkLastData("1");

    tearDown();
}

void test_publishOffCommand(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("SET/testPubOff0", sub);

    publishOffCommand("testPubOff0");
    checkLastData("0");

    tearDown();
}

void test_subscribeForLost(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForLost("testSubLost0", sub);
    subscribeForLost("testSubLost1", sub);


    publish((Posting) {.topic="testSubLost0/LOST", .data="testData0"});
    checkLastData("testData0");

    publish((Posting) {.topic="testSubLost1/LOST", .data="testData1"});
    checkLastData("testData1");

    tearDown();
}

void test_unsubscribeFromLost(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribeForLost("testUnsubLost0", sub);
    subscribeForLost("testUnsubLost1", sub);

    publish((Posting) {.topic="testUnsubLost0/LOST", .data="testData0"});
    checkLastData("testData0");

    publish((Posting) {.topic="testUnsubLost1/LOST", .data="testData1"});
    checkLastData("testData1");

    unsubscribeFromLost("testUnsubLost0", sub);
    publish((Posting) {.topic="testUnsubLost0/LOST", .data="testData0"});
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