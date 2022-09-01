#include "CommunicationEndpoint.h"
#include "ExampleLocalBroker.h"
#include "Posting.h"
#include "Subscriber.h"
#include "unity.h"
#include <string.h>

char *lastDelivered;

void setUp(void) {
    lastDelivered = "\n";
    init("eip://uni-due.de/es", "self");
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

void deliver(posting posting) {
    lastDelivered = posting.data;
}

void test_publishSubscribe(void) {
    Subscriber sub = (Subscriber){.deliver = deliver};
    CommunicationEndpointSubscribe("testPublishSub0", sub);
    CommunicationEndpointSubscribe("testPublishSub1", sub);

    CommunicationEndpointPublish((posting){.topic = "testPublishSub0", .data = "testData0"});
    checkLastData("testData0");

    CommunicationEndpointPublish((posting){.topic = "testPublishSub1", .data = "testData1"});
    checkLastData("testData1");
}

void test_publishUnsubscribe(void) {
    Subscriber sub = (Subscriber){.deliver = deliver};
    CommunicationEndpointSubscribe("testPublishUnsub0", sub);
    CommunicationEndpointSubscribe("testPublishUnsub1", sub);

    CommunicationEndpointPublish((posting){.topic = "testPublishUnsub0", .data = "testData0"});
    checkLastData("testData0");

    CommunicationEndpointPublish((posting){.topic = "testPublishUnsub1", .data = "testData1"});
    checkLastData("testData1");

    CommunicationEndpointUnsubscribe("testPublishUnsub0", sub);
    CommunicationEndpointPublish((posting){.topic = "testPublishUnsub0", .data = "testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    checkLastData("testData1");
}

void test_singleLevelWildcard(void) {
    Subscriber sub = (Subscriber){.deliver = deliver};

    CommunicationEndpointSubscribe("testSingleLeveLSub0/+/test", sub);
    CommunicationEndpointPublish(
        (posting){.topic = "testSingleLeveLSub0/abc/test", .data = "testData0"});
    checkLastData("testData0");

    CommunicationEndpointSubscribe("testSingleLeveLSub1/+/+", sub);
    CommunicationEndpointPublish(
        (posting){.topic = "testSingleLeveLSub1/abc/abc", .data = "testData1"});
    checkLastData("testData1");

    CommunicationEndpointSubscribe("+/+/testSingleLeveLSub2", sub);
    CommunicationEndpointPublish(
        (posting){.topic = "abc/abc/testSingleLeveLSub2", .data = "testData2"});
    checkLastData("testData2");
}

void test_multiLevelWildcard(void) {
    Subscriber sub = (Subscriber){.deliver = deliver};

    CommunicationEndpointSubscribe("testMultiLevelSub/#", sub);
    CommunicationEndpointPublish(
        (posting){.topic = "testMultiLevelSub/abc/abc", .data = "testData1"});
    checkLastData("testData1");
}

void test_singleLevelWildcardUnsubscribe(void) {

    Subscriber sub = (Subscriber){.deliver = deliver};

    CommunicationEndpointSubscribe("testSingleLevelUnsub/+/test", sub);
    CommunicationEndpointPublish(
        (posting){.topic = "testSingleLevelUnsub/abc/test", .data = "testData0"});
    checkLastData("testData0");

    CommunicationEndpointUnsubscribe("testSingleLevelUnsub/+/test", sub);
    CommunicationEndpointPublish(
        (posting){.topic = "testSingleLevelUnsub/abc/test", .data = "testData1"});
    checkLastData("testData0");
}

void test_multiLevelWildcardUnsubscribe(void) {
    Subscriber sub = (Subscriber){.deliver = deliver};

    CommunicationEndpointSubscribe("testMultiLevelUnsub/#", sub);
    CommunicationEndpointPublish(
        (posting){.topic = "testMultiLevelUnsub/abc/abc", .data = "testData0"});
    checkLastData("testData0");

    CommunicationEndpointUnsubscribe("testMultiLevelUnsub/#", sub);
    CommunicationEndpointPublish(
        (posting){.topic = "testMultiLevelUnsub/abc/abc", .data = "testData1"});
    checkLastData("testData0");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_publishSubscribe);
    RUN_TEST(test_publishUnsubscribe);

    RUN_TEST(test_singleLevelWildcard);
    RUN_TEST(test_multiLevelWildcard);
    RUN_TEST(test_singleLevelWildcardUnsubscribe);
    RUN_TEST(test_multiLevelWildcardUnsubscribe);

    return UNITY_END();
}
