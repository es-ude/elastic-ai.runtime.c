#include "unity.h"
#include <string.h>
#include "communicationEndpoint.h"
#include "ExampleLocalBroker.h"
#include "posting.h"
#include "protocol.h"
#include "stdlib.h"

Posting lastDelivered;

void setUp(void) {
    lastDelivered.topic = "\0";
    lastDelivered.data = "\0";
    setID("eip://uni-due.de/es/");
}

void tearDown(void) {
    lastDelivered.topic = "\0";
    lastDelivered.data = "\0";
}

void checkLastData(char *expected) {
    if (strcmp(expected, lastDelivered.data) != 0) {
        printf("EXPECTED:%s\n", expected);
        printf("ACTUAL:%s\n", lastDelivered);
        TEST_FAIL();
    }
}

void deliver(Posting posting) {
    lastDelivered.topic = posting.topic;
    lastDelivered.data = posting.data;
}

void test_publishSubscribeForData(void) {
    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForData("testSubData0", sub);
    subscribeForData("testSubData1", sub);

    publishData("testSubData0", "testData0");
    checkLastData("testData0");

    publishData("testSubData1", "testData1");
    checkLastData("testData1");
}

void test_publishUnsubscribeFromData(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribeForData("testUnsubData0", sub);
    subscribeForData("testUnsubData1", sub);

    publishData("testUnsubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishData("testUnsubData1", "testData1");
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromData("testUnsubData0", sub);
    publishData("testUnsubData0", "testData0");
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    tearDown();
}

void test_publishSubscribeForHeartbeat(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForHeartbeat("testSubHeart0", sub);
    subscribeForHeartbeat("testSubHeart1", sub);

    publishHeartbeat("testSubHeart0");
    TEST_ASSERT_EQUAL_STRING("testSubHeart0", lastDelivered.data);

    publishHeartbeat("testSubHeart1");
    TEST_ASSERT_EQUAL_STRING("testSubHeart1", lastDelivered.data);
    tearDown();
}

void test_publishUnsubscribeFromHeartbeat(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribeForHeartbeat("testUnsubHeart0", sub);
    subscribeForHeartbeat("testUnsubHeart1", sub);

    publishHeartbeat("testUnsubHeart0");
    TEST_ASSERT_EQUAL_STRING("testUnsubHeart0", lastDelivered.data);

    publishHeartbeat("testUnsubHeart1");
    TEST_ASSERT_EQUAL_STRING("testUnsubHeart1", lastDelivered.data);

    unsubscribeFromHeartbeat("testUnsubHeart0", sub);
    publishHeartbeat("testUnsubHeart0");
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testUnsubHeart1", lastDelivered.data);

    tearDown();
}

void test_subscribeForDataStartRequest(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForDataStartRequest("testSubDataStart0", sub);
    subscribeForDataStartRequest("testSubDataStart1", sub);

    publishDataStartRequest("testSubDataStart0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishDataStartRequest("testSubDataStart1", "testData1");
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    tearDown();
}

void test_subscribeForDataStopRequest(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForDataStopRequest("testSubDataStop0", sub);
    subscribeForDataStopRequest("testSubDataStop1", sub);

    publishDataStopRequest("testSubDataStop0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishDataStopRequest("testSubDataStop1", "testData1");
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    tearDown();
}

void test_publishCommand(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("SET/testPubCmd0", sub);
    subscribe("SET/testPubCmd1", sub);

    publishCommand("testPubCmd0", "0");
    TEST_ASSERT_EQUAL_STRING("0", lastDelivered.data);

    publishCommand("testPubCmd1", "1");
    TEST_ASSERT_EQUAL_STRING("1", lastDelivered.data);

    tearDown();
}


void test_publishOnCommand(void) {
    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("SET/testPubOn0", sub);

    publishOnCommand("testPubOn0");
    TEST_ASSERT_EQUAL_STRING("1", lastDelivered.data);
}

void test_publishOffCommand(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("SET/testPubOff0", sub);

    publishOffCommand("testPubOff0");
    TEST_ASSERT_EQUAL_STRING("0", lastDelivered.data);

    tearDown();
}

void test_subscribeForLost(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForLost("testSubLost0", sub);
    subscribeForLost("testSubLost1", sub);

    char *topic = malloc(strlen("testSubLost0/LOST"));
    Posting posting = (Posting) {.topic=topic, .data="testData0"};
    publish(posting);
    free(topic);

    publish((Posting) {.topic="LOST/testSubLost0", .data="testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publish((Posting) {.topic="LOST/testSubLost1", .data="testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    tearDown();
}

void test_unsubscribeFromLost(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribeForLost("testUnsubLost0", sub);
    subscribeForLost("testUnsubLost1", sub);

    publish((Posting) {.topic="LOST/testUnsubLost0", .data="testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publish((Posting) {.topic="LOST/testUnsubLost1", .data="testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromLost("testUnsubLost0", sub);
    publish((Posting) {.topic="LOST/testUnsubLost0", .data="testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

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
//
    RUN_TEST(test_subscribeForLost); // fails
    RUN_TEST(test_unsubscribeFromLost); // fails

    return UNITY_END();
}