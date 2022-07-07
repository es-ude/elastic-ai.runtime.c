#include "protocol.h"
#include "communicationEndpoint.h"
#include "posting.h"
#include <stdlib.h>
#include <string.h>

void addType(const char *type, const char *typeSpec, char *result) {
    strcpy(result, type);
    strcat(result, "/");
    strcat(result, typeSpec);
}

void subscribe_intern(char *type, char *topic, Subscriber subscriber) {
    char *result = malloc(strlen(type) + strlen(topic) + 2);
    addType(type, topic, result);
    subscribe(result, subscriber);
    free(result);
}

void unsubscribe_intern(char *type, char *topic, Subscriber subscriber) {
    char *result = malloc(strlen(type) + strlen(topic) + 2);
    addType(type, topic, result);
    unsubscribe(result, subscriber);
    free(result);
}

void subscribeForData(char *dataId, Subscriber subscriber) {
    subscribe_intern("DATA", dataId, subscriber);
}

void unsubscribeFromData(char *dataId, Subscriber subscriber) {
    unsubscribe_intern("DATA", dataId, subscriber);
}

void subscribeForHeartbeat(char *heartbeatSource, Subscriber subscriber) {
    subscribe_intern(heartbeatSource, "HEARTBEAT", subscriber);
}

void unsubscribeFromHeartbeat(char *heartbeatSource, Subscriber subscriber) {
    unsubscribe_intern(heartbeatSource, "HEARTBEAT", subscriber);
}

void subscribeForDataStartRequest(char *dataId, Subscriber subscriber) {
    subscribe_intern("START", dataId, subscriber);
}

void subscribeForDataStopRequest(char *dataId, Subscriber subscriber) {
    subscribe_intern("STOP", dataId, subscriber);
}

void subscribeForLost(char *client, Subscriber subscriber) {
    subscribe_intern("LOST", client, subscriber);
}

void unsubscribeFromLost(char *client, Subscriber subscriber) {
    unsubscribe_intern("LOST", client, subscriber);
}

void publish_intern(char *type, char *dataId, char *value) {
    char *topic = malloc(strlen(type) + strlen(dataId) + 2);
    addType(type, dataId, topic);
    Posting posting = (Posting){.topic = topic, .data = value};
    publish(posting);
    free(topic);
}

void publishData(char *dataId, char *value) {
    publish_intern("DATA", dataId, value);
}

void publishHeartbeat(char *who) {
    publish_intern(who, "HEARTBEAT", who);
}

void publishDataStartRequest(char *dataId, char *receiver) {
    publish_intern("START", dataId, receiver);
}

void publishDataStopRequest(char *dataId, char *receiver) {
    publish_intern("STOP", dataId, receiver);
}

void publishCommand(char *service, char *cmd) {
    publish_intern("SET", service, cmd);
}

void publishOnCommand(char *service) {
    publishCommand(service, "1");
}

void publishOffCommand(char *service) {
    publishCommand(service, "0");
}
