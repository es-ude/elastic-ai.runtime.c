#include "posting.h"
#include <stdlib.h>
#include <string.h>

char *addType(const char *type, const char *topic) {
    char *result = malloc(strlen(type) + strlen(topic) + 2);
    strcpy(result, type);
    strcat(result, "/");
    strcat(result, topic);
    return result;
}

Posting createTurnOn(char *topic) {
    return createCommand(topic, "1");
}

Posting createTurnOff(char *topic) {
    return createCommand(topic, "0");
}

Posting createCommand(char *topic, char *command) {
    return (Posting) {.topic= addType("SET", topic), .data=command};
}

Posting createStartSending(char *dataId, char *receiver) {
    return (Posting) {.topic= addType("START", dataId), .data=receiver};
}

Posting createStopSending(char *dataId, char *receiver) {
    return (Posting) {.topic= addType("STOP", dataId), .data=receiver};
}

Posting createData(char *phenomena, char *value) {
    return (Posting) {.topic= addType("DATA", phenomena), .data=value};
}

Posting createHeartbeat(char *heartbeatSource) {
    return (Posting) {.topic=addType(heartbeatSource, "HEARTBEAT"), .data=heartbeatSource};
}

//Posting cloneWithTopicAffix(char *affix) {
//    return Posting(affix + topic(), data());
//}

//boolean isStartSending(char *phenomena) {
//    return this.topic.contains(PostingType.START.topic(phenomena));
//}
