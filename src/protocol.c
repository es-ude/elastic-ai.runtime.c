#include "protocol.h"
#include "posting.h"
#include "communicationEndpoint.h"

void subscribeForData(char *dataId, Subscriber subscriber) {
    subscribe(addType("DATA", dataId), subscriber);
}

void unsubscribeFromData(char *dataId, Subscriber subscriber) {
    unsubscribe(addType("DATA", dataId), subscriber);
}

void publishData(char *dataId, char *value) {
    Posting post = createData(dataId, value);
    publish(post);
}

void subscribeForHeartbeat(char *heartbeatSource, Subscriber subscriber) {
    subscribe(addType(heartbeatSource, "HEARTBEAT"), subscriber);
}

void unsubscribeFromHeartbeat(char *heartbeatSource, Subscriber subscriber) {
    unsubscribe(addType(heartbeatSource, "HEARTBEAT"), subscriber);
}

void publishHeartbeat(char *who) {
    Posting post = createHeartbeat(who);
    publish(post);
}

void subscribeForDataStartRequest(char *dataId, Subscriber subscriber) {
    subscribe(addType("START", dataId), subscriber);
}

void subscribeForDataStopRequest(char *dataId, Subscriber subscriber) {
    subscribe(addType("STOP", dataId), subscriber);
}

void publishDataStartRequest(char *dataId, char *receiver) {
    Posting post = createStartSending(dataId, receiver);
    publish(post);
}

void publishDataStopRequest(char *dataId, char *receiver) {
    Posting post = createStopSending(dataId, receiver);
    publish(post);
}

void publishCommand(char *service, char *cmd) {
    Posting post = createCommand(service, cmd);
    publish(post);
}

void publishOnCommand(char *service) {
    Posting post = createTurnOn(service);
    publish(post);
}

void publishOffCommand(char *service) {
    Posting post = createTurnOff(service);
    publish(post);
}

void subscribeForLost(char *client, Subscriber subscriber) {
    subscribe(addType(client, "LOST"), subscriber);
}

void unsubscribeFromLost(char *client, Subscriber subscriber) {
    unsubscribe(addType(client, "LOST"), subscriber);
}
