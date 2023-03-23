#include "CommunicationEndpoint.h"
#include "ExampleLocalBroker.h"
#include "Posting.h"
#include "Protocol.h"
#include "unity.h"

posting_t lastDelivered;

void setUp(void) {
    lastDelivered.topic = "\0";
    lastDelivered.data = "\0";
    init("eip://uni-due.de/es", "self");
}

void tearDown(void) {
    lastDelivered.topic = "\0";
    lastDelivered.data = "\0";
}

void deliver(posting_t posting_t) {
    lastDelivered.topic = posting_t.topic;
    lastDelivered.data = posting_t.data;
}

subscriber_t subscriber = (subscriber_t){.deliver = deliver};
char *remoteTwin = "remote";

void test_publishData(void) {
    communicationEndpointSubscribeRaw("eip://uni-due.de/es/self/" DATA "/testPubData0", subscriber);

    protocolPublishData("testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointUnsubscribeRaw("eip://uni-due.de/es/self/" DATA "/testPubData0",
                                        subscriber);
}

void test_subscribeForData(void) {
    protocolSubscribeForData(remoteTwin, "testSubData0", subscriber);
    protocolSubscribeForData(remoteTwin, "testSubData1", subscriber);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/remote/" DATA "/testSubData0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/remote/" DATA "/testSubData1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromData(remoteTwin, "testSubData0", subscriber);
    protocolUnsubscribeFromData(remoteTwin, "testSubData1", subscriber);
}

void test_unsubscribeFromData(void) {
    protocolSubscribeForData(remoteTwin, "testUnsubData0", subscriber);
    protocolSubscribeForData(remoteTwin, "testUnsubData1", subscriber);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/remote/" DATA "/testUnsubData0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/remote/" DATA "/testUnsubData1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromData(remoteTwin, "testUnsubData0", subscriber);
    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/remote/" DATA "/testUnsubData0", .data = "testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromData(remoteTwin, "testUnsubData1", subscriber);
}

void test_publishHeartbeat(void) {
    communicationEndpointSubscribe(HEARTBEAT, subscriber);

    protocolPublishHeartbeat("testDevice");
    TEST_ASSERT_EQUAL_STRING("testDevice", lastDelivered.data);

    communicationEndpointUnsubscribe(HEARTBEAT, subscriber);
}

void test_subscribeForHeartbeat(void) {
    protocolSubscribeForHeartbeat("remote0", subscriber);
    protocolSubscribeForHeartbeat("remote1", subscriber);

    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote0/" HEARTBEAT, .data = "remote0"});
    TEST_ASSERT_EQUAL_STRING("remote0", lastDelivered.data);

    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote1/" HEARTBEAT, .data = "remote1"});
    TEST_ASSERT_EQUAL_STRING("remote1", lastDelivered.data);

    protocolUnsubscribeFromHeartbeat("remote0", subscriber);
    protocolUnsubscribeFromHeartbeat("remote1", subscriber);
}

void test_unsubscribeFromHeartbeat(void) {
    protocolSubscribeForHeartbeat("remote0", subscriber);
    protocolSubscribeForHeartbeat("remote1", subscriber);

    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote0/" HEARTBEAT, .data = "remote0"});
    TEST_ASSERT_EQUAL_STRING("remote0", lastDelivered.data);

    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote1/" HEARTBEAT, .data = "remote1"});
    TEST_ASSERT_EQUAL_STRING("remote1", lastDelivered.data);

    protocolUnsubscribeFromHeartbeat("remote0", subscriber);
    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote0/" HEARTBEAT, .data = "remote0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("remote1", lastDelivered.data);

    protocolUnsubscribeFromHeartbeat("remote1", subscriber);
}

void test_publishDataStartRequest(void) {
    communicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/" START "/testPubData0",
                                      subscriber);

    protocolPublishDataStartRequest("remote", "testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointUnsubscribeRaw("eip://uni-due.de/es/remote/" START "/testPubData0",
                                        subscriber);
}

void test_subscribeForDataStartRequest(void) {
    protocolSubscribeForDataStartRequest("testSubDataStart0", subscriber);
    protocolSubscribeForDataStartRequest("testSubDataStart1", subscriber);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" START "/testSubDataStart0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" START "/testSubDataStart1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromDataStartRequest("testSubDataStop0", subscriber);
    protocolUnsubscribeFromDataStartRequest("testSubDataStop1", subscriber);
}

void test_unsubscribeFromDataStartRequest(void) {
    protocolSubscribeForDataStartRequest("testUnsubDataStart0", subscriber);
    protocolSubscribeForDataStartRequest("testUnsubDataStart1", subscriber);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" START "/testUnsubDataStart0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" START "/testUnsubDataStart1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromDataStartRequest("testUnsubDataStart0", subscriber);
    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" START "/testUnsubDataStart0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromDataStartRequest("testUnsubDataStart1", subscriber);
}

void test_publishDataStopRequest(void) {
    communicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/" STOP "/testPubData0",
                                      subscriber);

    protocolPublishDataStopRequest(remoteTwin, "testPubData0", "testData0");
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointUnsubscribeRaw("eip://uni-due.de/es/remote/" STOP "/testPubData0",
                                        subscriber);
}

void test_subscribeForDataStopRequest(void) {
    protocolSubscribeForDataStopRequest("testSubDataStop0", subscriber);
    protocolSubscribeForDataStopRequest("testSubDataStop1", subscriber);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" STOP "/testSubDataStop0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointPublishRaw((posting_t){

        .topic = "eip://uni-due.de/es/self/" STOP "/testSubDataStop1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromDataStopRequest("testSubDataStop0", subscriber);
    protocolUnsubscribeFromDataStopRequest("testSubDataStop1", subscriber);
}

void test_unsubscribeFromDataStopRequest(void) {
    protocolSubscribeForDataStopRequest("testUnsubDataStop0", subscriber);
    protocolSubscribeForDataStopRequest("testUnsubDataStop1", subscriber);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" STOP "/testUnsubDataStop0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" STOP "/testUnsubDataStop1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromDataStopRequest("testUnsubDataStop0", subscriber);
    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" STOP "/testUnsubDataStop0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromDataStopRequest("testUnsubDataStop0", subscriber);
}

void test_publishStatus(void) {
    communicationEndpointSubscribeRaw("eip://uni-due.de/es/self/" STATUS, subscriber);

    protocolPublishStatus((status_t){.id = "ID", .state = STATUS_STATE_ONLINE});
    TEST_ASSERT_EQUAL_STRING("ID:ID;STATE:ONLINE;", lastDelivered.data);

    communicationEndpointUnsubscribeRaw("eip://uni-due.de/es/self/" STATUS, subscriber);
}

void test_subscribeForStatus(void) {
    protocolSubscribeForStatus(remoteTwin, subscriber);

    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote/" STATUS, .data = "testSubStatus0"});
    TEST_ASSERT_EQUAL_STRING("testSubStatus0", lastDelivered.data);

    protocolUnsubscribeFromStatus(remoteTwin, subscriber);
}

void test_unsubscribeFromStatus(void) {
    protocolSubscribeForStatus(remoteTwin, subscriber);

    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote/" STATUS, .data = "testUnsubStatus0"});
    TEST_ASSERT_EQUAL_STRING("testUnsubStatus0", lastDelivered.data);

    protocolUnsubscribeFromStatus(remoteTwin, subscriber);
    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote/" STATUS, .data = "testUnsubStatus1"});
    // Should not have changed as Subscriber is now longer subscribed too topic
    TEST_ASSERT_EQUAL_STRING("testUnsubStatus0", lastDelivered.data);
}

void test_publishCommand(void) {
    communicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/" COMMAND "/testPubCmd0",
                                      subscriber);
    communicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/" COMMAND "/testPubCmd1",
                                      subscriber);

    protocolPublishCommand("remote", "testPubCmd0", "0");
    TEST_ASSERT_EQUAL_STRING("0", lastDelivered.data);

    protocolPublishCommand("remote", "testPubCmd1", "1");
    TEST_ASSERT_EQUAL_STRING("1", lastDelivered.data);

    communicationEndpointUnsubscribeRaw("eip://uni-due.de/es/remote/" COMMAND "/testPubCmd0",
                                        subscriber);
    communicationEndpointUnsubscribeRaw("eip://uni-due.de/es/remote/" COMMAND "/testPubCmd1",
                                        subscriber);
}

void test_publishOnCommand(void) {
    communicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/" COMMAND "/testPubOn0",
                                      subscriber);

    protocolPublishOnCommand("remote", "testPubOn0");
    TEST_ASSERT_EQUAL_STRING("1", lastDelivered.data);

    communicationEndpointUnsubscribe("eip://uni-due.de/es/remote/" COMMAND "/testPubOn0",
                                     subscriber);
}

void test_publishOffCommand(void) {
    communicationEndpointSubscribeRaw("eip://uni-due.de/es/remote/" COMMAND "/testPubOff0",
                                      subscriber);

    protocolPublishOffCommand("remote", "testPubOff0");
    TEST_ASSERT_EQUAL_STRING("0", lastDelivered.data);

    communicationEndpointUnsubscribeRaw("eip://uni-due.de/es/remote/" COMMAND "/testPubOff0",
                                        subscriber);
}

void test_subscribeForCommand(void) {
    protocolSubscribeForCommand("testCommand0", subscriber);
    protocolSubscribeForCommand("testCommand1", subscriber);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" COMMAND "/testCommand0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" COMMAND "/testCommand1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromCommand("testCommand0", subscriber);
    protocolUnsubscribeFromCommand("testCommand1", subscriber);
}

void test_unsubscribeFromCommand(void) {
    protocolSubscribeForCommand("testCommand0", subscriber);
    protocolSubscribeForCommand("testCommand1", subscriber);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" COMMAND "/testCommand0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" COMMAND "/testCommand1", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromCommand("testCommand0", subscriber);
    communicationEndpointPublishRaw((posting_t){
        .topic = "eip://uni-due.de/es/self/" COMMAND "/testCommand0", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromCommand("testCommand1", subscriber);
}

void test_subscribeForLost(void) {
    protocolSubscribeForLost("remote0", subscriber);
    protocolSubscribeForLost("remote1", subscriber);

    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote0/" LOST "", .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote1/" LOST "", .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromLost("remote0", subscriber);
    protocolUnsubscribeFromLost("remote1", subscriber);
}

void test_unsubscribeFromLost(void) {
    protocolSubscribeForLost("remote0", subscriber);
    protocolSubscribeForLost("remote1", subscriber);

    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote0/" LOST, .data = "testData0"});
    TEST_ASSERT_EQUAL_STRING("testData0", lastDelivered.data);

    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote1/" LOST, .data = "testData1"});
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromLost("remote0", subscriber);
    communicationEndpointPublishRaw(
        (posting_t){.topic = "eip://uni-due.de/es/remote0/" LOST, .data = "testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    TEST_ASSERT_EQUAL_STRING("testData1", lastDelivered.data);

    protocolUnsubscribeFromLost("testUnsubLost1", subscriber);
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

    RUN_TEST(test_publishStatus);
    RUN_TEST(test_subscribeForStatus);
    RUN_TEST(test_unsubscribeFromStatus);

    RUN_TEST(test_publishCommand);
    RUN_TEST(test_publishOnCommand);
    RUN_TEST(test_publishOffCommand);
    RUN_TEST(test_subscribeForCommand);
    RUN_TEST(test_unsubscribeFromCommand);

    RUN_TEST(test_subscribeForLost);
    RUN_TEST(test_unsubscribeFromLost);

    return UNITY_END();
}
