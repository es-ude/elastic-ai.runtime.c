#include "../extern/unity/src/unity.h"
#include <string.h>
#include "posting.h"
#include "subscriber.h"
#include "communicationEndpoint.h"

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

void test_publishSubscribeRaw(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("testPublishSub0", sub);
    subscribe("testPublishSub1", sub);

    publish((Posting) {.topic="testPublishSub0", .data="testData0"});
    checkLastData("testData0");

    publish((Posting) {.topic="testPublishSub1", .data="testData1"});
    checkLastData("testData1");

    tearDown();
}

void test_publishUnsubscribeRaw(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribe("testPublishUnsub0", sub);
    subscribe("testPublishUnsub1", sub);

    publish((Posting) {.topic="testPublishUnsub0", .data="testData0"});
    checkLastData("testData0");

    publish((Posting) {.topic="testPublishUnsub1", .data="testData1"});
    checkLastData("testData1");

    unsubscribe("testPublishUnsub0", sub);
    publish((Posting) {.topic="testPublishUnsub0", .data="testData0"});
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    checkLastData("testData1");

    tearDown();
}

void test_singleLevelWildcard(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribe("testSingleLeveLSub0/+/test", sub);
    publish((Posting) {.topic="testSingleLeveLSub0/abc/test", .data="testData0"});
    checkLastData("testData0");

    subscribe("testSingleLeveLSub1/+/+", sub);
    publish((Posting) {.topic="testSingleLeveLSub1/abc/abc", .data="testData1"});
    checkLastData("testData1");

    subscribe("+/+/testSingleLeveLSub2", sub);
    publish((Posting) {.topic="abc/abc/testSingleLeveLSub2", .data="testData2"});
    checkLastData("testData2");

    tearDown();
}

void test_multiLevelWildcard(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribe("testMultiLevelSub/#", sub);
    publish((Posting) {.topic="testMultiLevelSub/abc/abc", .data="testData1"});
    checkLastData("testData1");

    tearDown();
}

void test_singleLevelWildcardUnsubscribe(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribe("testSingleLevelUnsub/+/test", sub);
    publish((Posting) {.topic="testSingleLevelUnsub/abc/test", .data="testData0"});
    checkLastData("testData0");

    unsubscribe("testSingleLevelUnsub/+/test", sub);
    publish((Posting) {.topic="testSingleLevelUnsub/abc/test", .data="testData1"});
    checkLastData("testData0");

    tearDown();
}

void test_multiLevelWildcardUnsubscribe(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribe("testMultiLevelUnsub/#", sub);
    publish((Posting) {.topic="testMultiLevelUnsub/abc/abc", .data="testData0"});
    checkLastData("testData0");

    unsubscribe("testMultiLevelUnsub/#", sub);
    publish((Posting) {.topic="testMultiLevelUnsub/abc/abc", .data="testData1"});
    checkLastData("testData0");

    tearDown();
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_publishSubscribeRaw);
    RUN_TEST(test_publishUnsubscribeRaw);

    RUN_TEST(test_singleLevelWildcard);
    RUN_TEST(test_multiLevelWildcard);
    RUN_TEST(test_singleLevelWildcardUnsubscribe);
    RUN_TEST(test_multiLevelWildcardUnsubscribe);

    return UNITY_END();
}
