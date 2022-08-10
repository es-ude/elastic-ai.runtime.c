#include "communicationEndpoint.h"
#include "exampleLocalBroker.h"
#include "posting.h"
#include "protocol.h"
#include "unity.h"

Posting lastDelivered;

void setUp(void) {
    lastDelivered.topic = "\0";
    lastDelivered.data = "\0";
    init("eip://uni-due.de/es", "self");
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
char *remoteTwin = "remote";

void test_publishData(void) {
    subscribeRaw("eip://uni-due.de/es/self/DATA/testPubData0", subscriber);

    publishData("testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    unsubscribeRaw("eip://uni-due.de/es/self/DATA/testPubData0", subscriber);
}

void test_subscribeForData(void) {
    subscribeForData(remoteTwin, "testSubData0", subscriber);
    subscribeForData(remoteTwin, "testSubData1", subscriber);

    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/remote/DATA/testSubData0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/remote/DATA/testSubData1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromData(remoteTwin, "testSubData0", subscriber);
    unsubscribeFromData(remoteTwin, "testSubData1", subscriber);
}

void test_unsubscribeFromData(void) {
    subscribeForData(remoteTwin, "testUnsubData0", subscriber);
    subscribeForData(remoteTwin, "testUnsubData1", subscriber);

    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/remote/DATA/testUnsubData0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/remote/DATA/testUnsubData1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromData(remoteTwin, "testUnsubData0", subscriber);
    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/remote/DATA/testUnsubData0", .data = "testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromData(remoteTwin, "testUnsubData1", subscriber);
}

void test_publishHeartbeat(void) {
    subscribe("HEART", subscriber);

    publishHeartbeat("testDevice");
    TEST_ASSERT_EQUAL_STRING("testDevice", lastDelivered.data);

    unsubscribe("HEART", subscriber);
}

void test_subscribeForHeartbeat(void) {
    subscribeForHeartbeat("remote0", subscriber);
    subscribeForHeartbeat("remote1", subscriber);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/remote0/HEART", .data = "remote0"});
    TEST_ASSERT_EQUAL_STRING("remote0", lastDelivered.data);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/remote1/HEART", .data = "remote1"});
    TEST_ASSERT_EQUAL_STRING("remote1", lastDelivered.data);

    unsubscribeFromHeartbeat("remote0", subscriber);
    unsubscribeFromHeartbeat("remote1", subscriber);
}

void test_unsubscribeFromHeartbeat(void) {
    subscribeForHeartbeat("remote0", subscriber);
    subscribeForHeartbeat("remote1", subscriber);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/remote0/HEART", .data = "remote0"});
    TEST_ASSERT_EQUAL_STRING("remote0", lastDelivered.data);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/remote1/HEART", .data = "remote1"});
    TEST_ASSERT_EQUAL_STRING("remote1", lastDelivered.data);

    unsubscribeFromHeartbeat("remote0", subscriber);
    publishRaw((Posting){.topic = "eip://uni-due.de/es/remote0/HEART", .data = "remote0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("remote1", lastDelivered.data);

    unsubscribeFromHeartbeat("remote1", subscriber);
}

void test_publishDataStartRequest(void) {
    subscribeRaw("eip://uni-due.de/es/remote/START/testPubData0", subscriber);

    publishDataStartRequest("remote", "testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    unsubscribeRaw("eip://uni-due.de/es/remote/START/testPubData0", subscriber);
}

void test_subscribeForDataStartRequest(void) {
    subscribeForDataStartRequest("testSubDataStart0", subscriber);
    subscribeForDataStartRequest("testSubDataStart1", subscriber);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/self/START/testSubDataStart0",
                         .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/self/START/testSubDataStart1",
                         .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromDataStartRequest("testSubDataStop0", subscriber);
    unsubscribeFromDataStartRequest("testSubDataStop1", subscriber);
}

void test_unsubscribeFromDataStartRequest(void) {
    subscribeForDataStartRequest("testUnsubDataStart0", subscriber);
    subscribeForDataStartRequest("testUnsubDataStart1", subscriber);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/self/START/testUnsubDataStart0",
                         .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/self/START/testUnsubDataStart1",
                         .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromDataStartRequest("testUnsubDataStart0", subscriber);
    publishRaw((Posting){.topic = "eip://uni-due.de/es/self/START/testUnsubDataStart0",
                         .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromDataStartRequest("testUnsubDataStart1", subscriber);
}

void test_publishDataStopRequest(void) {
    subscribeRaw("eip://uni-due.de/es/remote/STOP/testPubData0", subscriber);

    publishDataStopRequest(remoteTwin, "testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    unsubscribeRaw("eip://uni-due.de/es/remote/STOP/testPubData0", subscriber);
}

void test_subscribeForDataStopRequest(void) {
    subscribeForDataStopRequest("testSubDataStop0", subscriber);
    subscribeForDataStopRequest("testSubDataStop1", subscriber);

    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/self/STOP/testSubDataStop0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/self/STOP/testSubDataStop1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromDataStopRequest("testSubDataStop0", subscriber);
    unsubscribeFromDataStopRequest("testSubDataStop1", subscriber);
}

void test_unsubscribeFromDataStopRequest(void) {
    subscribeForDataStopRequest("testUnsubDataStop0", subscriber);
    subscribeForDataStopRequest("testUnsubDataStop1", subscriber);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/self/STOP/testUnsubDataStop0",
                         .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/self/STOP/testUnsubDataStop1",
                         .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromDataStopRequest("testUnsubDataStop0", subscriber);
    publishRaw((Posting){.topic = "eip://uni-due.de/es/self/STOP/testUnsubDataStop0",
                         .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromDataStopRequest("testUnsubDataStop0", subscriber);
}

void test_publishCommand(void) {
    subscribeRaw("eip://uni-due.de/es/remote/SET/testPubCmd0", subscriber);
    subscribeRaw("eip://uni-due.de/es/remote/SET/testPubCmd1", subscriber);

    publishCommand("remote", "testPubCmd0", "0");
    TEST_ASSERT_EQUAL_STRING("0", lastDelivered.data);

    publishCommand("remote", "testPubCmd1", "1");
    TEST_ASSERT_EQUAL_STRING("1", lastDelivered.data);

    unsubscribeRaw("eip://uni-due.de/es/remote/SET/testPubCmd0", subscriber);
    unsubscribeRaw("eip://uni-due.de/es/remote/SET/testPubCmd1", subscriber);
}

void test_publishOnCommand(void) {
    subscribeRaw("eip://uni-due.de/es/remote/SET/testPubOn0", subscriber);

    publishOnCommand("remote", "testPubOn0");
    TEST_ASSERT_EQUAL_STRING("1", lastDelivered.data);

    unsubscribe("eip://uni-due.de/es/remote/SET/testPubOn0", subscriber);
}

void test_publishOffCommand(void) {
    subscribeRaw("eip://uni-due.de/es/remote/SET/testPubOff0", subscriber);

    publishOffCommand("remote", "testPubOff0");
    TEST_ASSERT_EQUAL_STRING("0", lastDelivered.data);

    unsubscribeRaw("eip://uni-due.de/es/remote/SET/testPubOff0", subscriber);
}

void test_subscribeForCommand(void) {
    subscribeForCommand("testCommand0", subscriber);
    subscribeForCommand("testCommand1", subscriber);

    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/self/SET/testCommand0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/self/SET/testCommand1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromCommand("testCommand0", subscriber);
    unsubscribeFromCommand("testCommand1", subscriber);
}

void test_unsubscribeFromCommand(void) {
    subscribeForCommand("testCommand0", subscriber);
    subscribeForCommand("testCommand1", subscriber);

    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/self/SET/testCommand0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/self/SET/testCommand1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromCommand("testCommand0", subscriber);
    publishRaw(
        (Posting){.topic = "eip://uni-due.de/es/self/SET/testCommand0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromCommand("testCommand1", subscriber);
}

void test_subscribeForLost(void) {
    subscribeForLost("remote0", subscriber);
    subscribeForLost("remote1", subscriber);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/remote0/LOST", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/remote1/LOST", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromLost("remote0", subscriber);
    unsubscribeFromLost("remote1", subscriber);
}

void test_unsubscribeFromLost(void) {
    subscribeForLost("remote0", subscriber);
    subscribeForLost("remote1", subscriber);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/remote0/LOST", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    publishRaw((Posting){.topic = "eip://uni-due.de/es/remote1/LOST", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromLost("remote0", subscriber);
    publishRaw((Posting){.topic = "eip://uni-due.de/es/remote0/LOST", .data = "testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    unsubscribeFromLost("testUnsubLost1", subscriber);
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
    RUN_TEST(test_unsubscribeFromDataStartRequest);

    RUN_TEST(test_publishDataStopRequest);
    RUN_TEST(test_subscribeForDataStopRequest);
    RUN_TEST(test_unsubscribeFromDataStopRequest);

    RUN_TEST(test_publishCommand);
    RUN_TEST(test_publishOnCommand);
    RUN_TEST(test_publishOffCommand);
    RUN_TEST(test_subscribeForCommand);
    RUN_TEST(test_unsubscribeFromCommand);

    RUN_TEST(test_subscribeForLost);
    RUN_TEST(test_unsubscribeFromLost);

    return UNITY_END();
}
