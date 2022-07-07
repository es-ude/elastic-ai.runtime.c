#include "protocol.h"
#include "communicationEndpoint.h"
#include "posting.h"
#include <stdlib.h>
#include <string.h>

void publish_intern(char *type, char *dataId, char *value);

char *addType(const char *type, const char *data);

char *getTopic(const char *twin, const char *type, const char *data);

void subscribe_intern(char *twin, char *type, char *data, Subscriber subscriber);

void unsubscribe_intern(char *twin, char *type, char *data, Subscriber subscriber);

void subscribeForData(char *twin, char *dataId, Subscriber subscriber) {
    subscribe_intern(twin, DATA, dataId, subscriber);
}

void unsubscribeFromData(char *twin, char *dataId, Subscriber subscriber) {
    unsubscribe_intern(twin, DATA, dataId, subscriber);
}

void subscribeForHeartbeat(char *heartbeatSource, Subscriber subscriber) {
    subscribe_intern(heartbeatSource, HEARTBEAT, "", subscriber);
}

void unsubscribeFromHeartbeat(char *heartbeatSource, Subscriber subscriber) {
    unsubscribe_intern(heartbeatSource, HEARTBEAT, "", subscriber);
}

void subscribeForDataStartRequest(char *twin, char *dataId, Subscriber subscriber) {
    subscribe_intern(twin, START, dataId, subscriber);
}

void subscribeForDataStopRequest(char *twin, char *dataId, Subscriber subscriber) {
    subscribe_intern(twin, STOP, dataId, subscriber);
}

void subscribeForLost(char *twin, char *client, Subscriber subscriber) {
    subscribe_intern(twin, LOST, client, subscriber);
}

void unsubscribeFromLost(char *twin, char *client, Subscriber subscriber) {
    unsubscribe_intern(twin, LOST, client, subscriber);
}

void subscribe_intern(char *twin, char *type, char *data, Subscriber subscriber) {
    char *result = getTopic(twin, type, data);
    subscribe(result, subscriber);
    free(result);
}

void unsubscribe_intern(char *twin, char *type, char *data, Subscriber subscriber) {
    char *result = getTopic(twin, type, data);
    unsubscribe(result, subscriber);
    free(result);
}

char *getTopic(const char *twin, const char *type, const char *data) {
    int slashNum = 3;
    if (strlen(data) == 0) {
        slashNum--;
    }
    char *result = malloc(strlen(twin) + strlen(type) + strlen(data) + slashNum + 1);
    strcpy(result, twin);
    strcat(result, "/");
    strcat(result, type);
    if (strlen(data) != 0) {
        strcat(result, "/");
        strcat(result, data);
    }
    return result;
}

void publishData(char *dataId, char *value) {
    publish_intern(DATA, dataId, value);
}

void publishHeartbeat(char *who) {
    publish_intern(HEARTBEAT, "", who);
}

void publishDataStartRequest(char *dataId, char *receiver) {
    publish_intern(START, dataId, receiver);
}

void publishDataStopRequest(char *dataId, char *receiver) {
    publish_intern(STOP, dataId, receiver);
}

void publishCommand(char *service, char *cmd) {
    publish_intern(SET, service, cmd);
}

void publishOnCommand(char *service) {
    publishCommand(service, "1");
}

void publishOffCommand(char *service) {
    publishCommand(service, "0");
}

void publish_intern(char *type, char *dataId, char *value) {
    char *topic = addType(type, dataId);
    Posting posting = (Posting){.topic = topic, .data = value};
    publish(posting);
    free(topic);
}

char *addType(const char *type, const char *data) {
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
