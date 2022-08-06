#include "protocol.h"
#include "communicationEndpoint.h"
#include "posting.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void publish_intern(char *type, char *dataId, char *value);

void subscribe_intern(char *type, char *data, Subscriber subscriber);

void unsubscribe_intern(char *type, char *data, Subscriber subscriber);

void publishRemote_intern(char *twin, char *type, char *dataId, char *value);

void subscribeRemote_intern(char *twin, char *type, char *data, Subscriber subscriber);

void unsubscribeRemote_intern(char *twin, char *type, char *data, Subscriber subscriber);

char *addType(const char *type, const char *data);

char *getTopic(const char *twin, const char *type, const char *data);

/*** SELF ***/
void publish_intern(char *type, char *dataId, char *value) {
    char *topic = addType(type, dataId);
    Posting posting = (Posting){.topic = topic, .data = value};
    publish(posting);
    free(topic);
}

void subscribe_intern(char *type, char *data, Subscriber subscriber) {
    char *result = addType(type, data);
    subscribe(result, subscriber);
}

void unsubscribe_intern(char *type, char *data, Subscriber subscriber) {
    char *result = addType(type, data);
    unsubscribe(result, subscriber);
    free(result);
}

void publishData(char *dataId, char *value) {
    publish_intern(DATA, dataId, value);
}

void publishHeartbeat(char *who) {
    publish_intern(HEARTBEAT, "", who);
}

void subscribeForDataStartRequest(char *dataId, Subscriber subscriber) {
    subscribe_intern(START, dataId, subscriber);
}

void unsubscribeFromDataStartRequest(char *dataId, Subscriber subscriber) {
    unsubscribe_intern(START, dataId, subscriber);
}

void subscribeForDataStopRequest(char *dataId, Subscriber subscriber) {
    subscribe_intern(STOP, dataId, subscriber);
}

void unsubscribeFromDataStopRequest(char *dataId, Subscriber subscriber) {
    unsubscribe_intern(STOP, dataId, subscriber);
}

void subscribeForCommand(char *dataId, Subscriber subscriber) {
    subscribe_intern(COMMAND, dataId, subscriber);
}

void unsubscribeFromCommand(char *dataId, Subscriber subscriber) {
    unsubscribe_intern(COMMAND, dataId, subscriber);
}

/*** Remote ***/
void publishRemote_intern(char *twin, char *type, char *dataId, char *value) {
    char *topic = getTopic(twin, type, dataId);
    Posting posting = (Posting){.topic = topic, .data = value};
    publishRemote(posting);
    free(topic);
}

void subscribeRemote_intern(char *twin, char *type, char *data, Subscriber subscriber) {
    char *result = getTopic(twin, type, data);
    subscribeRemote(result, subscriber);
    free(result);
}

void unsubscribeRemote_intern(char *twin, char *type, char *data, Subscriber subscriber) {
    char *result = getTopic(twin, type, data);
    unsubscribeRemote(result, subscriber);
    free(result);
}

void publishDataStartRequest(char *twin, char *dataId, char *receiver) {
    publishRemote_intern(twin, START, dataId, receiver);
}

void publishDataStopRequest(char *twin, char *dataId, char *receiver) {
    publishRemote_intern(twin, STOP, dataId, receiver);
}

void publishCommand(char *twin, char *service, char *cmd) {
    publishRemote_intern(twin, COMMAND, service, cmd);
}

void publishOnCommand(char *twin, char *service) {
    publishCommand(twin, service, "1");
}

void publishOffCommand(char *twin, char *service) {
    publishCommand(twin, service, "0");
}

void subscribeForData(char *twin, char *dataId, Subscriber subscriber) {
    subscribeRemote_intern(twin, DATA, dataId, subscriber);
}

void unsubscribeFromData(char *twin, char *dataId, Subscriber subscriber) {
    unsubscribeRemote_intern(twin, DATA, dataId, subscriber);
}

void subscribeForHeartbeat(char *heartbeatSource, Subscriber subscriber) {
    subscribeRemote_intern(heartbeatSource, HEARTBEAT, "", subscriber);
}

void unsubscribeFromHeartbeat(char *heartbeatSource, Subscriber subscriber) {
    unsubscribeRemote_intern(heartbeatSource, HEARTBEAT, "", subscriber);
}

void subscribeForLost(char *twin, Subscriber subscriber) {
    subscribeRemote_intern(twin, LOST, "", subscriber);
}

void unsubscribeFromLost(char *twin, Subscriber subscriber) {
    unsubscribeRemote_intern(twin, LOST, "", subscriber);
}

/*** HELPER ***/
char *getTopic(const char *twin, const char *type, const char *data) {
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
