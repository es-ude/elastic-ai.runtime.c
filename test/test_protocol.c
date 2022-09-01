#include "CommunicationEndpoint.h"
#include "ExampleLocalBroker.h"
#include "Posting.h"
#include "Protocol.h"
#include "unity.h"

posting lastDelivered;

void setUp(void) {
    lastDelivered.topic = "\0";
    lastDelivered.data = "\0";
    init("eip://uni-due.de/es", "self");
}

void tearDown(void) {
    lastDelivered.topic = "\0";
    lastDelivered.data = "\0";
}

void deliver(posting posting) {
    lastDelivered.topic = posting.topic;
    lastDelivered.data = posting.data;
}

Subscriber subscriber = (Subscriber){.deliver = deliver};
char *remoteTwin = "remote";

void test_publishData(void) {
    CommunicationEndpointSubscribeRaw("eip://uni-due.de/es/self/DATA/testPubData0", subscriber);

    ProtocolPublishData("testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointUnsubscribeRaw("eip://uni-due.de/es/self/DATA/testPubData0", subscriber);
}

void test_subscribeForData(void) {
    ProtocolSubscribeForData(remoteTwin, "testSubData0", subscriber);
    ProtocolSubscribeForData(remoteTwin, "testSubData1", subscriber);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote/DATA/testSubData0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote/DATA/testSubData1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromData(remoteTwin, "testSubData0", subscriber);
    ProtocolUnsubscribeFromData(remoteTwin, "testSubData1", subscriber);
}

void test_unsubscribeFromData(void) {
    ProtocolSubscribeForData(remoteTwin, "testUnsubData0", subscriber);
    ProtocolSubscribeForData(remoteTwin, "testUnsubData1", subscriber);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote/DATA/testUnsubData0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote/DATA/testUnsubData1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromData(remoteTwin, "testUnsubData0", subscriber);
    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote/DATA/testUnsubData0", .data = "testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromData(remoteTwin, "testUnsubData1", subscriber);
}

void test_publishHeartbeat(void) {
    CommunicationEndpointSubscribe("HEART", subscriber);

    ProtocolPublishHeartbeat("testDevice");
    TEST_ASSERT_EQUAL_STRING("testDevice", lastDelivered.data);

    CommunicationEndpointUnsubscribe("HEART", subscriber);
}

void test_subscribeForHeartbeat(void) {
    ProtocolSubscribeForHeartbeat("remote0", subscriber);
    ProtocolSubscribeForHeartbeat("remote1", subscriber);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote0/HEART", .data = "remote0"});
    TEST_ASSERT_EQUAL_STRING("remote0", lastDelivered.data);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote1/HEART", .data = "remote1"});
    TEST_ASSERT_EQUAL_STRING("remote1", lastDelivered.data);

    ProtocolUnsubscribeFromHeartbeat("remote0", subscriber);
    ProtocolUnsubscribeFromHeartbeat("remote1", subscriber);
}

void test_unsubscribeFromHeartbeat(void) {
    ProtocolSubscribeForHeartbeat("remote0", subscriber);
    ProtocolSubscribeForHeartbeat("remote1", subscriber);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote0/HEART", .data = "remote0"});
    TEST_ASSERT_EQUAL_STRING("remote0", lastDelivered.data);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote1/HEART", .data = "remote1"});
    TEST_ASSERT_EQUAL_STRING("remote1", lastDelivered.data);

    ProtocolUnsubscribeFromHeartbeat("remote0", subscriber);
    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote0/HEART", .data = "remote0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("remote1", lastDelivered.data);

    ProtocolUnsubscribeFromHeartbeat("remote1", subscriber);
}

void test_publishDataStartRequest(void) {
    CommunicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/START/testPubData0", subscriber);

    ProtocolPublishDataStartRequest("remote", "testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointUnsubscribeRaw("eip://uni-due.de/es/remote/START/testPubData0",
                                        subscriber);
}

void test_subscribeForDataStartRequest(void) {
    ProtocolSubscribeForDataStartRequest("testSubDataStart0", subscriber);
    ProtocolSubscribeForDataStartRequest("testSubDataStart1", subscriber);

    CommunicationEndpointPublishRaw((posting){
        .topic = "eip://uni-due.de/es/self/START/testSubDataStart0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointPublishRaw((posting){
        .topic = "eip://uni-due.de/es/self/START/testSubDataStart1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromDataStartRequest("testSubDataStop0", subscriber);
    ProtocolUnsubscribeFromDataStartRequest("testSubDataStop1", subscriber);
}

void test_unsubscribeFromDataStartRequest(void) {
    ProtocolSubscribeForDataStartRequest("testUnsubDataStart0", subscriber);
    ProtocolSubscribeForDataStartRequest("testUnsubDataStart1", subscriber);

    CommunicationEndpointPublishRaw((posting){
        .topic = "eip://uni-due.de/es/self/START/testUnsubDataStart0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointPublishRaw((posting){
        .topic = "eip://uni-due.de/es/self/START/testUnsubDataStart1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromDataStartRequest("testUnsubDataStart0", subscriber);
    CommunicationEndpointPublishRaw((posting){
        .topic = "eip://uni-due.de/es/self/START/testUnsubDataStart0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromDataStartRequest("testUnsubDataStart1", subscriber);
}

void test_publishDataStopRequest(void) {
    CommunicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/STOP/testPubData0", subscriber);

    ProtocolPublishDataStopRequest(remoteTwin, "testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointUnsubscribeRaw("eip://uni-due.de/es/remote/STOP/testPubData0", subscriber);
}

void test_subscribeForDataStopRequest(void) {
    ProtocolSubscribeForDataStopRequest("testSubDataStop0", subscriber);
    ProtocolSubscribeForDataStopRequest("testSubDataStop1", subscriber);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/self/STOP/testSubDataStop0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/self/STOP/testSubDataStop1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromDataStopRequest("testSubDataStop0", subscriber);
    ProtocolUnsubscribeFromDataStopRequest("testSubDataStop1", subscriber);
}

void test_unsubscribeFromDataStopRequest(void) {
    ProtocolSubscribeForDataStopRequest("testUnsubDataStop0", subscriber);
    ProtocolSubscribeForDataStopRequest("testUnsubDataStop1", subscriber);

    CommunicationEndpointPublishRaw((posting){
        .topic = "eip://uni-due.de/es/self/STOP/testUnsubDataStop0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointPublishRaw((posting){
        .topic = "eip://uni-due.de/es/self/STOP/testUnsubDataStop1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromDataStopRequest("testUnsubDataStop0", subscriber);
    CommunicationEndpointPublishRaw((posting){
        .topic = "eip://uni-due.de/es/self/STOP/testUnsubDataStop0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromDataStopRequest("testUnsubDataStop0", subscriber);
}

void test_publishCommand(void) {
    CommunicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/SET/testPubCmd0", subscriber);
    CommunicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/SET/testPubCmd1", subscriber);

    ProtocolPublishCommand("remote", "testPubCmd0", "0");
    TEST_ASSERT_EQUAL_STRING("0", lastDelivered.data);

    ProtocolPublishCommand("remote", "testPubCmd1", "1");
    TEST_ASSERT_EQUAL_STRING("1", lastDelivered.data);

    CommunicationEndpointUnsubscribeRaw("eip://uni-due.de/es/remote/SET/testPubCmd0", subscriber);
    CommunicationEndpointUnsubscribeRaw("eip://uni-due.de/es/remote/SET/testPubCmd1", subscriber);
}

void test_publishOnCommand(void) {
    CommunicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/SET/testPubOn0", subscriber);

    ProtocolPublishOnCommand("remote", "testPubOn0");
    TEST_ASSERT_EQUAL_STRING("1", lastDelivered.data);

    CommunicationEndpointUnsubscribe("eip://uni-due.de/es/remote/SET/testPubOn0", subscriber);
}

void test_publishOffCommand(void) {
    CommunicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/SET/testPubOff0", subscriber);

    ProtocolPublishOffCommand("remote", "testPubOff0");
    TEST_ASSERT_EQUAL_STRING("0", lastDelivered.data);

    CommunicationEndpointUnsubscribeRaw("eip://uni-due.de/es/remote/SET/testPubOff0", subscriber);
}

void test_subscribeForCommand(void) {
    ProtocolSubscribeForCommand("testCommand0", subscriber);
    ProtocolSubscribeForCommand("testCommand1", subscriber);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/self/SET/testCommand0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/self/SET/testCommand1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromCommand("testCommand0", subscriber);
    ProtocolUnsubscribeFromCommand("testCommand1", subscriber);
}

void test_unsubscribeFromCommand(void) {
    ProtocolSubscribeForCommand("testCommand0", subscriber);
    ProtocolSubscribeForCommand("testCommand1", subscriber);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/self/SET/testCommand0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/self/SET/testCommand1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromCommand("testCommand0", subscriber);
    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/self/SET/testCommand0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromCommand("testCommand1", subscriber);
}

void test_subscribeForLost(void) {
    ProtocolSubscribeForLost("remote0", subscriber);
    ProtocolSubscribeForLost("remote1", subscriber);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote0/LOST", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote1/LOST", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromLost("remote0", subscriber);
    ProtocolUnsubscribeFromLost("remote1", subscriber);
}

void test_unsubscribeFromLost(void) {
    ProtocolSubscribeForLost("remote0", subscriber);
    ProtocolSubscribeForLost("remote1", subscriber);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote0/LOST", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote1/LOST", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromLost("remote0", subscriber);
    CommunicationEndpointPublishRaw(
        (posting){.topic = "eip://uni-due.de/es/remote0/LOST", .data = "testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    ProtocolUnsubscribeFromLost("testUnsubLost1", subscriber);
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
