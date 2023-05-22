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

void deliver(posting_t posting) {
    lastDelivered = posting.data;
}

void test_publishSubscribe(void) {
    subscriber_t sub = (subscriber_t){.deliver = deliver};
    communicationEndpointSubscribe("testPublishSub0", sub);
    communicationEndpointSubscribe("testPublishSub1", sub);

    communicationEndpointPublish((posting_t){.topic = "testPublishSub0", .data = "testData0"});
    checkLastData("testData0");

    communicationEndpointPublish((posting_t){.topic = "testPublishSub1", .data = "testData1"});
    checkLastData("testData1");
}

void test_publishUnsubscribe(void) {
    subscriber_t sub = (subscriber_t){.deliver = deliver};
    communicationEndpointSubscribe("testPublishUnsub0", sub);
    communicationEndpointSubscribe("testPublishUnsub1", sub);

    communicationEndpointPublish((posting_t){.topic = "testPublishUnsub0", .data = "testData0"});
    checkLastData("testData0");

    communicationEndpointPublish((posting_t){.topic = "testPublishUnsub1", .data = "testData1"});
    checkLastData("testData1");

    communicationEndpointUnsubscribe("testPublishUnsub0", sub);
    communicationEndpointPublish((posting_t){.topic = "testPublishUnsub0", .data = "testData0"});
    // Should not have changed as subscriber_t is now longer subscribed too topic: test0
    checkLastData("testData1");
}

void test_singleLevelWildcard(void) {
    subscriber_t sub = (subscriber_t){.deliver = deliver};

    communicationEndpointSubscribe("testSingleLeveLSub0/+/test", sub);
    communicationEndpointPublish(
        (posting_t){.topic = "testSingleLeveLSub0/abc/test", .data = "testData0"});
    checkLastData("testData0");

    communicationEndpointSubscribe("testSingleLeveLSub1/+/+", sub);
    communicationEndpointPublish(
        (posting_t){.topic = "testSingleLeveLSub1/abc/abc", .data = "testData1"});
    checkLastData("testData1");

    communicationEndpointSubscribe("+/+/testSingleLeveLSub2", sub);
    communicationEndpointPublish(
        (posting_t){.topic = "abc/abc/testSingleLeveLSub2", .data = "testData2"});
    checkLastData("testData2");
}

void test_multiLevelWildcard(void) {
    subscriber_t sub = (subscriber_t){.deliver = deliver};

    communicationEndpointSubscribe("testMultiLevelSub/#", sub);
    communicationEndpointPublish(
        (posting_t){.topic = "testMultiLevelSub/abc/abc", .data = "testData1"});
    checkLastData("testData1");
}

void test_singleLevelWildcardUnsubscribe(void) {

    subscriber_t sub = (subscriber_t){.deliver = deliver};

    communicationEndpointSubscribe("testSingleLevelUnsub/+/test", sub);
    communicationEndpointPublish(
        (posting_t){.topic = "testSingleLevelUnsub/abc/test", .data = "testData0"});
    checkLastData("testData0");

    communicationEndpointUnsubscribe("testSingleLevelUnsub/+/test", sub);
    communicationEndpointPublish(
        (posting_t){.topic = "testSingleLevelUnsub/abc/test", .data = "testData1"});
    checkLastData("testData0");
}

void test_multiLevelWildcardUnsubscribe(void) {
    subscriber_t sub = (subscriber_t){.deliver = deliver};

    communicationEndpointSubscribe("testMultiLevelUnsub/#", sub);
    communicationEndpointPublish(
        (posting_t){.topic = "testMultiLevelUnsub/abc/abc", .data = "testData0"});
    checkLastData("testData0");

    communicationEndpointUnsubscribe("testMultiLevelUnsub/#", sub);
    communicationEndpointPublish(
        (posting_t){.topic = "testMultiLevelUnsub/abc/abc", .data = "testData1"});
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
