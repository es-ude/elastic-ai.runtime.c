#ifndef ELASTIC_AI_RUNTIME_C_PROTOCOL_H
#define ELASTIC_AI_RUNTIME_C_PROTOCOL_H

#include "subscriber.h"

void subscribeForData(char *dataId, Subscriber subscriber);

void unsubscribeFromData(char *dataId, Subscriber subscriber);

void publishData(char *dataId, char *value);

void subscribeForHeartbeat(char *heartbeatSource, Subscriber subscriber);

void unsubscribeFromHeartbeat(char *heartbeatSource, Subscriber subscriber);

void publishHeartbeat(char *who);

void subscribeForDataStartRequest(char *dataId, Subscriber subscriber);

void subscribeForDataStopRequest(char *dataId, Subscriber subscriber);

void publishDataStartRequest(char *dataId, char *receiver);

void publishDataStopRequest(char *dataId, char *receiver);

void publishCommand(char *service, char *cmd);

void publishOnCommand(char *service);

void publishOffCommand(char *service);

void subscribeForLost(char *client, Subscriber subscriber);

void unsubscribeFromLost(char *client, Subscriber subscriber);

#endif //ELASTIC_AI_RUNTIME_C_PROTOCOL_H