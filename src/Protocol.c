#include "Protocol.h"
#include "CommunicationEndpoint.h"
#include "Posting.h"
#include "ProtocolInternal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* region SELF */

void ProtocolPublishData(char *dataId, char *value) {
    ProtocolInternPublish(DATA, dataId, value);
}

void ProtocolPublishHeartbeat(char *who) {
    ProtocolInternPublish(HEARTBEAT, "", who);
}

void ProtocolSubscribeForDataStartRequest(char *dataId, subscriber_t subscriber) {
    ProtocolInternSubscribe(START, dataId, subscriber);
}

void ProtocolUnsubscribeFromDataStartRequest(char *dataId, subscriber_t subscriber) {
    ProtocolInternUnsubscribe(START, dataId, subscriber);
}

void ProtocolSubscribeForDataStopRequest(char *dataId, subscriber_t subscriber) {
    ProtocolInternSubscribe(STOP, dataId, subscriber);
}

void ProtocolUnsubscribeFromDataStopRequest(char *dataId, subscriber_t subscriber) {
    ProtocolInternUnsubscribe(STOP, dataId, subscriber);
}

void ProtocolSubscribeForCommand(char *dataId, subscriber_t subscriber) {
    ProtocolInternSubscribe(COMMAND, dataId, subscriber);
}

void ProtocolUnsubscribeFromCommand(char *dataId, subscriber_t subscriber) {
    ProtocolInternUnsubscribe(COMMAND, dataId, subscriber);
}

/* endregion SELF */

/* region REMOTE */

void ProtocolPublishDataStartRequest(char *twin, char *dataId, char *receiver) {
    ProtocolInternPublishRemote(twin, START, dataId, receiver);
}

void ProtocolPublishDataStopRequest(char *twin, char *dataId, char *receiver) {
    ProtocolInternPublishRemote(twin, STOP, dataId, receiver);
}

void ProtocolPublishCommand(char *twin, char *service, char *cmd) {
    ProtocolInternPublishRemote(twin, COMMAND, service, cmd);
}

void ProtocolPublishOnCommand(char *twin, char *service) {
    ProtocolPublishCommand(twin, service, "1");
}

void ProtocolPublishOffCommand(char *twin, char *service) {
    ProtocolPublishCommand(twin, service, "0");
}

void ProtocolSubscribeForData(char *twin, char *dataId, subscriber_t subscriber) {
    ProtocolInternSubscribeRemote(twin, DATA, dataId, subscriber);
}

void ProtocolUnsubscribeFromData(char *twin, char *dataId, subscriber_t subscriber) {
    ProtocolInternUnsubscribeRemote(twin, DATA, dataId, subscriber);
}

void ProtocolSubscribeForHeartbeat(char *heartbeatSource, subscriber_t subscriber) {
    ProtocolInternSubscribeRemote(heartbeatSource, HEARTBEAT, "", subscriber);
}

void ProtocolUnsubscribeFromHeartbeat(char *heartbeatSource, subscriber_t subscriber) {
    ProtocolInternUnsubscribeRemote(heartbeatSource, HEARTBEAT, "", subscriber);
}

void ProtocolSubscribeForLost(char *twin, subscriber_t subscriber) {
    ProtocolInternSubscribeRemote(twin, LOST, "", subscriber);
}

void ProtocolUnsubscribeFromLost(char *twin, subscriber_t subscriber) {
    ProtocolInternUnsubscribeRemote(twin, LOST, "", subscriber);
}

/* endregion REMOTE */

/* region INTERNAL HEADER FUNCTIONS */

void ProtocolInternPublishRemote(char *twin, char *type, char *dataId, char *valueToPulish) {
    char *topic = ProtocolInternGetTopic(twin, type, dataId);
    posting_t posting = (posting_t){.topic = topic, .data = valueToPulish};
    CommunicationEndpointPublishRemote(posting);
    free(topic);
}

void ProtocolInternSubscribeRemote(char *twin, char *type, char *data, subscriber_t subscriber) {
    char *result = ProtocolInternGetTopic(twin, type, data);
    CommunicationEndpointSubscribeRemote(result, subscriber);
    free(result);
}

void ProtocolInternUnsubscribeRemote(char *twin, char *type, char *data, subscriber_t subscriber) {
    char *result = ProtocolInternGetTopic(twin, type, data);
    CommunicationEndpointUnsubscribeRemote(result, subscriber);
    free(result);
}
void ProtocolInternPublish(char *type, char *dataId, char *valueToPublish) {
    char *topic = ProtocolInternAddType(type, dataId);
    posting_t posting = (posting_t){.topic = topic, .data = valueToPublish};
    CommunicationEndpointPublish(posting);
    free(topic);
}

void ProtocolInternSubscribe(char *type, char *data, subscriber_t subscriber) {
    char *result = ProtocolInternAddType(type, data);
    CommunicationEndpointSubscribe(result, subscriber);
}

void ProtocolInternUnsubscribe(char *type, char *data, subscriber_t subscriber) {
    char *result = ProtocolInternAddType(type, data);
    CommunicationEndpointUnsubscribe(result, subscriber);
    free(result);
}

char *ProtocolInternGetTopic(const char *twin, const char *type, const char *data) {
    int slashNum = 3;
    if (strlen(data) == 0) {
        slashNum--;
    }
    uint16_t length = strlen(twin) + strlen(type) + strlen(data) + slashNum + 1;
    char *result = malloc(length);
    snprintf(result, length, "%s/%s", twin, type);
    if (strlen(data) != 0) {
        strcat(result, "/");
        strcat(result, data);
    }
    return result;
}

char *ProtocolInternAddType(const char *type, const char *data) {
    if (strlen(data) == 0) {
        char *result = malloc(strlen(type) + 1);
        strcpy(result, type);
        return result;
    }
    char *result = malloc(strlen(type) + strlen(data) + 2);
    strcpy(result, type);
    strcat(result, "/");
    strcat(result, data);
    return result;
}

/* endregion INTERNAL HEADER FUNCTIONS */
