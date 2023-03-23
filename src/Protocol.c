#include "Protocol.h"
#include "CommunicationEndpoint.h"
#include "Posting.h"
#include "ProtocolInternal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void addToMessage(char **information, char *informationElement, char *value) {
    if (value > 0) {
        char *newInfo =
            malloc(strlen(*information) + strlen(value) + strlen(informationElement) + 3);
        strcpy(newInfo, *information);
        strcat(newInfo, informationElement);
        strcat(newInfo, ":");
        strcat(newInfo, value);
        strcat(newInfo, ";");
        *information = newInfo;
    }
}

char *getStatusMessage(status_t status) {
    char *information = malloc(strlen(status.id) + strlen(STATUS_ID) + 3);
    strcpy(information, STATUS_ID);
    strcat(information, ":");
    strcat(information, status.id);
    strcat(information, ";");

    addToMessage(&information, STATUS_TYPE, status.type);
    addToMessage(&information, STATUS_STATE, status.state);
    addToMessage(&information, STATUS_MEASUREMENTS, status.measurements);

    return information;
}

/* region SELF */

void protocolPublishData(char *dataId, char *valueToPublish) {
    protocolInternPublish(DATA, dataId, valueToPublish, false);
}

void protocolPublishHeartbeat(char *who) {
    protocolInternPublish(HEARTBEAT, "", who, false);
}

void protocolSubscribeForDataStartRequest(char *dataId, subscriber_t subscriber) {
    protocolInternSubscribe(START, dataId, subscriber);
}

void protocolUnsubscribeFromDataStartRequest(char *dataId, subscriber_t subscriber) {
    protocolInternUnsubscribe(START, dataId, subscriber);
}

void protocolSubscribeForDataStopRequest(char *dataId, subscriber_t subscriber) {
    protocolInternSubscribe(STOP, dataId, subscriber);
}

void protocolUnsubscribeFromDataStopRequest(char *dataId, subscriber_t subscriber) {
    protocolInternUnsubscribe(STOP, dataId, subscriber);
}

void protocolSubscribeForCommand(char *dataId, subscriber_t subscriber) {
    protocolInternSubscribe(COMMAND, dataId, subscriber);
}

void protocolUnsubscribeFromCommand(char *dataId, subscriber_t subscriber) {
    protocolInternUnsubscribe(COMMAND, dataId, subscriber);
}

void protocolPublishStatus(status_t status) {
    protocolInternPublish(STATUS, "", getStatusMessage(status), true);
}

/* endregion SELF */

/* region REMOTE */

void protocolPublishDataStartRequest(char *twin, char *dataId, char *receiver) {
    protocolInternPublishRemote(twin, START, dataId, receiver);
}

void protocolPublishDataStopRequest(char *twin, char *dataId, char *receiver) {
    protocolInternPublishRemote(twin, STOP, dataId, receiver);
}

void protocolPublishCommand(char *twin, char *service, char *cmd) {
    protocolInternPublishRemote(twin, COMMAND, service, cmd);
}

void protocolPublishOnCommand(char *twin, char *service) {
    protocolPublishCommand(twin, service, "1");
}

void protocolPublishOffCommand(char *twin, char *service) {
    protocolPublishCommand(twin, service, "0");
}

void protocolSubscribeForData(char *twin, char *dataId, subscriber_t subscriber) {
    protocolInternSubscribeRemote(twin, DATA, dataId, subscriber);
}

void protocolUnsubscribeFromData(char *twin, char *dataId, subscriber_t subscriber) {
    protocolInternUnsubscribeRemote(twin, DATA, dataId, subscriber);
}

void protocolSubscribeForHeartbeat(char *heartbeatSource, subscriber_t subscriber) {
    protocolInternSubscribeRemote(heartbeatSource, HEARTBEAT, "", subscriber);
}

void protocolUnsubscribeFromHeartbeat(char *heartbeatSource, subscriber_t subscriber) {
    protocolInternUnsubscribeRemote(heartbeatSource, HEARTBEAT, "", subscriber);
}

void protocolSubscribeForLost(char *twin, subscriber_t subscriber) {
    protocolInternSubscribeRemote(twin, LOST, "", subscriber);
}

void protocolUnsubscribeFromLost(char *twin, subscriber_t subscriber) {
    protocolInternUnsubscribeRemote(twin, LOST, "", subscriber);
}

void protocolSubscribeForStatus(char *twin, subscriber_t subscriber) {
    protocolInternSubscribeRemote(twin, STATUS, "", subscriber);
}

void protocolUnsubscribeFromStatus(char *twin, subscriber_t subscriber) {
    protocolInternUnsubscribeRemote(twin, STATUS, "", subscriber);
}

/* endregion REMOTE */

/* region INTERNAL HEADER FUNCTIONS */

void protocolInternPublishRemote(char *twin, char *type, char *dataId, char *valueToPublish) {
    char *topic = protocolInternGetTopic(twin, type, dataId);
    posting_t posting = (posting_t){.topic = topic, .data = valueToPublish};
    communicationEndpointPublishRemote(posting);
    free(topic);
}

void protocolInternSubscribeRemote(char *twin, char *type, char *data, subscriber_t subscriber) {
    char *result = protocolInternGetTopic(twin, type, data);
    communicationEndpointSubscribeRemote(result, subscriber);
    free(result);
}

void protocolInternUnsubscribeRemote(char *twin, char *type, char *data, subscriber_t subscriber) {
    char *result = protocolInternGetTopic(twin, type, data);
    communicationEndpointUnsubscribeRemote(result, subscriber);
    free(result);
}
void protocolInternPublish(char *type, char *dataId, char *valueToPublish, bool retain) {
    char *topic = protocolInternAddType(type, dataId);
    posting_t posting = (posting_t){.topic = topic, .data = valueToPublish, .retain = retain};
    communicationEndpointPublish(posting);
    free(topic);
}

void protocolInternSubscribe(char *type, char *data, subscriber_t subscriber) {
    char *result = protocolInternAddType(type, data);
    communicationEndpointSubscribe(result, subscriber);
}

void protocolInternUnsubscribe(char *type, char *data, subscriber_t subscriber) {
    char *result = protocolInternAddType(type, data);
    communicationEndpointUnsubscribe(result, subscriber);
    free(result);
}

char *protocolInternGetTopic(const char *twin, const char *type, const char *data) {
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

char *protocolInternAddType(const char *type, const char *data) {
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
