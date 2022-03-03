#include "../extern/unity/src/unity.h"
#include <string.h>
#include "communicationEndpoint.h"
#include "posting.h"
#include "protocol.h"

char *lastDeliveredData;

void setUp(void) {
    lastDeliveredData = "\n";
}

void tearDown(void) {
    lastDeliveredData = "\n";
}

void checkLastData(char *expected) {
    TEST_ASSERT_EQUAL(expected, lastDeliveredData);
}

void deliver(Posting posting) {
    lastDeliveredData = posting.data;
    //printf("%s\n", posting.data);
}

void test_publishSubscribeForData(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForData("test0", sub);
    subscribeForData("test1", sub);

    publishData("test0", "testData0");
    checkLastData("testData0");

    publishData("test1", "testData1");
    checkLastData("testData1");

    tearDown();
}

void test_publishUnsubscribeFromData(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};

    subscribeForData("test0", sub);
    subscribeForData("test1", sub);

    publishData("test0", "testData0");
    checkLastData("testData0");

    publishData("test1", "testData1");
    checkLastData("testData1");

    unsubscribeFromData("test0", sub);
    publishData("test0", "testData0");
    // Should not have changed as Subscriber is now longer subscribed too topic: test0
    checkLastData("testData1");

    tearDown();
}

void test_subscribeForDataStartRequest(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForDataStartRequest("test0", sub);
    subscribeForDataStartRequest("test1", sub);

    publishDataStartRequest("test0", "testData0");
    checkLastData("testData0");

    publishDataStartRequest("test1", "testData1");
    checkLastData("testData1");

    tearDown();
}

void test_subscribeForDataStopRequest(void) {
    setUp();

    Subscriber sub = (Subscriber) {.deliver=deliver};
    subscribeForDataStopRequest("test0", sub);
    subscribeForDataStopRequest("test1", sub);

    publishDataStopRequest("test0", "testData0");
    checkLastData("testData0");

    publishDataStopRequest("test1", "testData1");
    checkLastData("testData1");

    tearDown();
}

//void test_publishSubscribeForHeartbeat(void) {
//    Subscriber sub = (Subscriber) {.deliver=func};
//    subscribeForHeartbeat("test0", sub);
//    subscribeForHeartbeat("test1", sub);
//
//    publishHeartbeat("test0");
//    if (strcmp("testData0") != 0) {
//        TEST_FAIL();
//    }
//    publishHeartbeat("test1");
//    if (strcmp("testData1") != 0) {
//        TEST_FAIL();
//    }
//}
//
//void test_publishUnsubscribeFromHeartbeat(void) {
//    Subscriber sub = (Subscriber) {.deliver=func};
//
//    subscribeForHeartbeat("test0", sub);
//    subscribeForHeartbeat("test1", sub);
//
//    publishHeartbeat("test0");
//    if (strcmp("testData0") != 0) {
//        TEST_FAIL();
//    }
//    publishHeartbeat("test1");
//    if (strcmp("testData1") != 0) {
//        TEST_FAIL();
//    }
//    unsubscribeFromHeartbeat("test0", sub);
//    publishHeartbeat("test0"); //Should not change the pointer as no longer subscribed
//    if (strcmp("testData1") != 0) {
//        TEST_FAIL();
//    }
//}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_publishSubscribeForData);
    RUN_TEST(test_publishUnsubscribeFromData);

    RUN_TEST(test_subscribeForDataStartRequest);
    RUN_TEST(test_subscribeForDataStopRequest);

//    RUN_TEST(test_publishSubscribeForHeartbeat);
//    RUN_TEST(test_publishUnsubscribeFromHeartbeat);



    return UNITY_END();
}