#include "../extern/unity/src/unity.h"
#include "posting.h"

void setUp(void) {
}

void tearDown(void) {
}

void testCreateCommand(void) {
    char *topic = "topic";
    char *data = "data";

    Posting expected = (Posting) {.topic="SET/topic", .data="data"};
    Posting returned = createCommand(topic, data);

    TEST_ASSERT_EQUAL_STRING(expected.topic, returned.topic);
    TEST_ASSERT_EQUAL_STRING(expected.data, returned.data);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testCreateCommand);

    return UNITY_END();
}
