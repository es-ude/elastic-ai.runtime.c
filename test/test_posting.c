#include "../extern/unity/src/unity.h"
#include "posting.h"

void setUp(void) {
}

void tearDown(void) {
}

void testTurnOn(void) {
    char *topic = "topic";

    Posting expected = (Posting) {.topic="SET/topic", .data="1"};
    Posting returned = createTurnOn(topic);

    TEST_ASSERT_EQUAL_STRING(expected.topic, returned.topic);
    TEST_ASSERT_EQUAL_STRING(expected.data, returned.data);
}

void testTurnOff(void) {
    char *topic = "topic";

    Posting expected = (Posting) {.topic="SET/topic", .data="0"};
    Posting returned = createTurnOff(topic);

    TEST_ASSERT_EQUAL_STRING(expected.topic, returned.topic);
    TEST_ASSERT_EQUAL_STRING(expected.data, returned.data);
}

void testCreateCommand(void) {
    char *topic = "topic";
    char *data = "data";

    Posting expected = (Posting) {.topic="SET/topic", .data="data"};
    Posting returned = createCommand(topic, data);

    TEST_ASSERT_EQUAL_STRING(expected.topic, returned.topic);
    TEST_ASSERT_EQUAL_STRING(expected.data, returned.data);
}

void testCreateStartSending(void) {
    char *topic = "topic";
    char *data = "data";

    Posting expected = (Posting) {.topic="START/topic", .data="data"};
    Posting returned = createStartSending(topic, data);

    TEST_ASSERT_EQUAL_STRING(expected.topic, returned.topic);
    TEST_ASSERT_EQUAL_STRING(expected.data, returned.data);
}

void testCreateStopSending(void) {
    char *topic = "topic";
    char *data = "data";

    Posting expected = (Posting) {.topic="STOP/topic", .data="data"};
    Posting returned = createStopSending(topic, data);

    TEST_ASSERT_EQUAL_STRING(expected.topic, returned.topic);
    TEST_ASSERT_EQUAL_STRING(expected.data, returned.data);
}

void testCreateData(void) {
    char *topic = "topic";
    char *data = "data";

    Posting expected = (Posting) {.topic="DATA/topic", .data="data"};
    Posting returned = createData(topic, data);

    TEST_ASSERT_EQUAL_STRING(expected.topic, returned.topic);
    TEST_ASSERT_EQUAL_STRING(expected.data, returned.data);
}

void testCreateHeartbeat(void) {
    char *topic = "topic";

    Posting expected = (Posting) {.topic="topic/HEARTBEAT", .data="topic"};
    Posting returned = createHeartbeat(topic);

    TEST_ASSERT_EQUAL_STRING(expected.topic, returned.topic);
    TEST_ASSERT_EQUAL_STRING(expected.data, returned.data);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testTurnOn);
    RUN_TEST(testTurnOff);
    RUN_TEST(testCreateCommand);
    RUN_TEST(testCreateStartSending);
    RUN_TEST(testCreateStopSending);
    RUN_TEST(testCreateData);
    RUN_TEST(testCreateHeartbeat);

    return UNITY_END();
}
