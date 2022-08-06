#ifndef ELASTIC_AI_RUNTIME_C_PROTOCOL_H
#define ELASTIC_AI_RUNTIME_C_PROTOCOL_H

#include "subscriber.h"

#define DATA "DATA"
#define START "START"
#define STOP "STOP"
#define COMMAND "SET"
#define LOST "LOST"
#define HEARTBEAT "HEART"

/*** SELF ***/
void publishData(char *dataId, char *value);

void publishHeartbeat(char *who);

void subscribeForDataStartRequest(char *dataId, Subscriber subscriber);

void unsubscribeFromDataStartRequest(char *dataId, Subscriber subscriber);

void subscribeForDataStopRequest(char *dataId, Subscriber subscriber);

void unsubscribeFromDataStopRequest(char *dataId, Subscriber subscriber);

void subscribeForCommand(char *dataId, Subscriber subscriber);

void unsubscribeFromCommand(char *dataId, Subscriber subscriber);

/*** Remote ***/
void publishDataStartRequest(char *twin, char *dataId, char *receiver);

void publishDataStopRequest(char *twin, char *dataId, char *receiver);

void publishCommand(char *twin, char *service, char *cmd);

void publishOnCommand(char *twin, char *service);

void publishOffCommand(char *twin, char *service);

void subscribeForData(char *twin, char *dataId, Subscriber subscriber);

void unsubscribeFromData(char *twin, char *dataId, Subscriber subscriber);

void subscribeForHeartbeat(char *heartbeatSource, Subscriber subscriber);

void unsubscribeFromHeartbeat(char *heartbeatSource, Subscriber subscriber);

void subscribeForLost(char *twin, Subscriber subscriber);

void unsubscribeFromLost(char *twin, Subscriber subscriber);

#endif // ELASTIC_AI_RUNTIME_C_PROTOCOL_H
