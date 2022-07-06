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

void deliver(Posting posting) {
    lastDelivered.topic = posting.topic;
    lastDelivered.data = posting.data;
}

Subscriber subscriber = (Subscriber) {.deliver=deliver};

void test_publishSubscribeForData(void) {
    subscribeForData("testSubData0", subscriber);
    subscribeForData("testSubData1", subscriber);

    publishData("testSubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishData("testSubData1", "testData1");
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromData("testSubData0", subscriber);
    unsubscribeFromData("testSubData1", subscriber);
}

void test_publishUnsubscribeFromData(void) {
    subscribeForData("testUnsubData0", subscriber);
    subscribeForData("testUnsubData1", subscriber);

    publishData("testUnsubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishData("testUnsubData1", "testData1");
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromData("testUnsubData0", subscriber);
    publishData("testUnsubData0", "testData0");
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromData("testUnsubData1", subscriber);
}

void test_publishSubscribeForHeartbeat(void) {
    subscribeForHeartbeat("testSubHeart0", subscriber);
    subscribeForHeartbeat("testSubHeart1", subscriber);

    publishHeartbeat("testSubHeart0");
    TEST_ASSERT_EQUAL_STRING("testSubHeart0", lastDelivered.data);

    publishHeartbeat("testSubHeart1");
    TEST_ASSERT_EQUAL_STRING("testSubHeart1", lastDelivered.data);

    unsubscribeFromHeartbeat("testUnsubHeart0", subscriber);
    unsubscribeFromHeartbeat("testUnsubHeart1", subscriber);
}

void test_publishUnsubscribeFromHeartbeat(void) {
    subscribeForHeartbeat("testUnsubHeart0", subscriber);
    subscribeForHeartbeat("testUnsubHeart1", subscriber);

    publishHeartbeat("testUnsubHeart0");
    TEST_ASSERT_EQUAL_STRING("testUnsubHeart0", lastDelivered.data);

    publishHeartbeat("testUnsubHeart1");
    TEST_ASSERT_EQUAL_STRING("testUnsubHeart1", lastDelivered.data);

    unsubscribeFromHeartbeat("testUnsubHeart0", subscriber);
    publishHeartbeat("testUnsubHeart0");
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testUnsubHeart1", lastDelivered.data);

    unsubscribeFromHeartbeat("testUnsubHeart1", subscriber);
}

void test_subscribeForDataStartRequest(void) {
    subscribeForDataStartRequest("testSubDataStart0", subscriber);
    subscribeForDataStartRequest("testSubDataStart1", subscriber);

    publishDataStartRequest("testSubDataStart0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishDataStartRequest("testSubDataStart1", "testData1");
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribe("START/testSubDataStop0", subscriber);
    unsubscribe("START/testSubDataStop1", subscriber);
}

void test_subscribeForDataStopRequest(void) {
    subscribeForDataStopRequest("testSubDataStop0", subscriber);
    subscribeForDataStopRequest("testSubDataStop1", subscriber);

    publishDataStopRequest("testSubDataStop0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishDataStopRequest("testSubDataStop1", "testData1");
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribe("STOP/testSubDataStop0", subscriber);
    unsubscribe("STOP/testSubDataStop1", subscriber);
}

void test_publishCommand(void) {
    subscribe("SET/testPubCmd0", subscriber);
    subscribe("SET/testPubCmd1", subscriber);

    publishCommand("testPubCmd0", "0");
    TEST_ASSERT_EQUAL_STRING("0", lastDelivered.data);

    publishCommand("testPubCmd1", "1");
    TEST_ASSERT_EQUAL_STRING("1", lastDelivered.data);

    unsubscribe("SET/testPubCmd0", subscriber);
    unsubscribe("SET/testPubCmd1", subscriber);
}


void test_publishOnCommand(void) {
    subscribe("SET/testPubOn0", subscriber);

    publishOnCommand("testPubOn0");
    TEST_ASSERT_EQUAL_STRING("1", lastDelivered.data);

    unsubscribe("SET/testPubOn0", subscriber);
}

void test_publishOffCommand(void) {
    subscribe("SET/testPubOff0", subscriber);

    publishOffCommand("testPubOff0");
    TEST_ASSERT_EQUAL_STRING("0", lastDelivered.data);

    unsubscribe("SET/testPubOff0", subscriber);
}

void test_subscribeForLost(void) {
    subscribeForLost("testSubLost0", subscriber);
    subscribeForLost("testSubLost1", subscriber);

    char *topic = malloc(strlen("testSubLost0/LOST"));
    Posting posting = (Posting) {.topic=topic, .data="testData0"};
    publish(posting);
    free(topic);

    publish((Posting) {.topic="LOST/testSubLost0", .data="testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publish((Posting) {.topic="LOST/testSubLost1", .data="testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromLost("testUnsubLost0", subscriber);
    unsubscribeFromLost("testUnsubLost1", subscriber);
}

void test_unsubscribeFromLost(void) {
    subscribeForLost("testUnsubLost0", subscriber);
    subscribeForLost("testUnsubLost1", subscriber);

    publish((Posting) {.topic="LOST/testUnsubLost0", .data="testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publish((Posting) {.topic="LOST/testUnsubLost1", .data="testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromLost("testUnsubLost0", subscriber);
    publish((Posting) {.topic="LOST/testUnsubLost0", .data="testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromLost("testUnsubLost1", subscriber);
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
