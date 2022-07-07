#include "communicationEndpoint.h"
#include "posting.h"
#include "protocol.h"
#include "unity.h"

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

Subscriber subscriber = (Subscriber){.deliver = deliver};
char *twinID = "test";

void test_publishData(void) {
    subscribe("DATA/testPubData0", subscriber);

    publishData("testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    unsubscribe("DATA/testPubData0", subscriber);
}

void test_subscribeForData(void) {
    subscribeForData(twinID, "testSubData0", subscriber);
    subscribeForData(twinID, "testSubData1", subscriber);

    publish((Posting){.topic = "test/DATA/testSubData0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publish((Posting){.topic = "test/DATA/testSubData1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromData(twinID, "testSubData0", subscriber);
    unsubscribeFromData(twinID, "testSubData1", subscriber);
}

void test_unsubscribeFromData(void) {
    subscribeForData(twinID, "testUnsubData0", subscriber);
    subscribeForData(twinID, "testUnsubData1", subscriber);

    publish((Posting){.topic = "test/DATA/testUnsubData0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publish((Posting){.topic = "test/DATA/testUnsubData1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromData(twinID, "testUnsubData0", subscriber);
    publish((Posting){.topic = "test/DATA/testUnsubData0", .data = "testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromData(twinID, "testUnsubData1", subscriber);
}

void test_publishHeartbeat(void) {
    subscribe("HEART", subscriber);

    publishHeartbeat("testDevice");
    TEST_ASSERT_EQUAL_STRING("testDevice", lastDelivered.data);

    unsubscribe("HEART", subscriber);
}

void test_subscribeForHeartbeat(void) {
    subscribeForHeartbeat("testSubHeart0", subscriber);
    subscribeForHeartbeat("testSubHeart1", subscriber);

    publish((Posting){.topic = "testSubHeart0/HEART", .data = "testSubHeart0"});
    TEST_ASSERT_EQUAL_STRING("testSubHeart0", lastDelivered.data);

    publish((Posting){.topic = "testSubHeart1/HEART", .data = "testSubHeart1"});
    TEST_ASSERT_EQUAL_STRING("testSubHeart1", lastDelivered.data);

    unsubscribeFromHeartbeat("testUnsubHeart0", subscriber);
    unsubscribeFromHeartbeat("testUnsubHeart1", subscriber);
}

void test_unsubscribeFromHeartbeat(void) {
    subscribeForHeartbeat("testUnsubHeart0", subscriber);
    subscribeForHeartbeat("testUnsubHeart1", subscriber);

    publish((Posting){.topic = "testUnsubHeart0/HEART", .data = "testUnsubHeart0"});
    TEST_ASSERT_EQUAL_STRING("testUnsubHeart0", lastDelivered.data);

    publish((Posting){.topic = "testUnsubHeart1/HEART", .data = "testUnsubHeart1"});
    TEST_ASSERT_EQUAL_STRING("testUnsubHeart1", lastDelivered.data);

    unsubscribeFromHeartbeat("testUnsubHeart0", subscriber);
    publish((Posting){.topic = "testUnsubHeart0/HEART", .data = "testUnsubHeart0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testUnsubHeart1", lastDelivered.data);

    unsubscribeFromHeartbeat("testUnsubHeart1", subscriber);
}

void test_publishDataStartRequest(void) {
    subscribe("START/testPubData0", subscriber);

    publishDataStartRequest("testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    unsubscribe("START/testPubData0", subscriber);
}

void test_subscribeForDataStartRequest(void) {
    subscribeForDataStartRequest(twinID, "testSubDataStart0", subscriber);
    subscribeForDataStartRequest(twinID, "testSubDataStart1", subscriber);

    publish((Posting){.topic = "test/START/testSubDataStart0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publish((Posting){.topic = "test/START/testSubDataStart1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribe("test/START/testSubDataStop0", subscriber);
    unsubscribe("test/START/testSubDataStop1", subscriber);
}

void test_publishDataStopRequest(void) {
    subscribe("STOP/testPubData0", subscriber);

    publishDataStopRequest("testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    unsubscribe("STOP/testPubData0", subscriber);
}

void test_subscribeForDataStopRequest(void) {
    subscribeForDataStopRequest(twinID, "testSubDataStop0", subscriber);
    subscribeForDataStopRequest(twinID, "testSubDataStop1", subscriber);

    publish((Posting){.topic = "test/STOP/testSubDataStop0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publish((Posting){.topic = "test/STOP/testSubDataStop1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribe("test/STOP/testSubDataStop0", subscriber);
    unsubscribe("test/STOP/testSubDataStop1", subscriber);
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

    unsubscribe("test/SET/testPubOff0", subscriber);
}

void test_subscribeForLost(void) {
    subscribeForLost(twinID, "testSubLost0", subscriber);
    subscribeForLost(twinID, "testSubLost1", subscriber);

    publish((Posting){.topic = "test/LOST/testSubLost0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publish((Posting){.topic = "test/LOST/testSubLost1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromLost(twinID, "testUnsubLost0", subscriber);
    unsubscribeFromLost(twinID, "testUnsubLost1", subscriber);
}

void test_unsubscribeFromLost(void) {
    subscribeForLost(twinID, "testUnsubLost0", subscriber);
    subscribeForLost(twinID, "testUnsubLost1", subscriber);

    publish((Posting){.topic = "test/LOST/testUnsubLost0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publish((Posting){.topic = "test/LOST/testUnsubLost1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromLost(twinID, "testUnsubLost0", subscriber);
    publish((Posting){.topic = "test/LOST/testUnsubLost0", .data = "testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromLost(twinID, "testUnsubLost1", subscriber);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_publishData);
    RUN_TEST(test_subscribeForData);
    RUN_TEST(test_unsubscribeFromData);

    RUN_TEST(test_publishHeartbeat);
    RUN_TEST(test_subscribeForHeartbeat);
    RUN_TEST(test_unsubscribeFromHeartbeat);

    RUN_TEST(test_publishDataStartRequest);
    RUN_TEST(test_subscribeForDataStartRequest);

    RUN_TEST(test_publishDataStopRequest);
    RUN_TEST(test_subscribeForDataStopRequest);

    RUN_TEST(test_publishCommand);
    RUN_TEST(test_publishOnCommand);
    RUN_TEST(test_publishOffCommand);

    RUN_TEST(test_subscribeForLost);
    RUN_TEST(test_unsubscribeFromLost);

    return UNITY_END();
}
