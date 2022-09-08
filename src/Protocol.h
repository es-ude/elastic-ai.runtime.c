#ifndef ELASTIC_AI_RUNTIME_C_PROTOCOL_H
#define ELASTIC_AI_RUNTIME_C_PROTOCOL_H

#include "Subscriber.h"

#define DATA "DATA"
#define START "START"
#define STOP "STOP"
#define COMMAND "SET"
#define LOST "LOST"
#define HEARTBEAT "HEART"

/* region SELF */

void ProtocolPublishData(char *dataId, char *value);

void ProtocolPublishHeartbeat(char *who);

void ProtocolSubscribeForDataStartRequest(char *dataId, subscriber_t subscriber);

void ProtocolUnsubscribeFromDataStartRequest(char *dataId, subscriber_t subscriber);

void ProtocolSubscribeForDataStopRequest(char *dataId, subscriber_t subscriber);

void ProtocolUnsubscribeFromDataStopRequest(char *dataId, subscriber_t subscriber);

void ProtocolSubscribeForCommand(char *dataId, subscriber_t subscriber);

void ProtocolUnsubscribeFromCommand(char *dataId, subscriber_t subscriber);

/* endregion */

/*region REMOTE */

void ProtocolPublishDataStartRequest(char *twin, char *dataId, char *receiver);

void ProtocolPublishDataStopRequest(char *twin, char *dataId, char *receiver);

void ProtocolPublishCommand(char *twin, char *service, char *cmd);

void ProtocolPublishOnCommand(char *twin, char *service);

void ProtocolPublishOffCommand(char *twin, char *service);

void ProtocolSubscribeForData(char *twin, char *dataId, subscriber_t subscriber);

void ProtocolUnsubscribeFromData(char *twin, char *dataId, subscriber_t subscriber);

void ProtocolSubscribeForHeartbeat(char *heartbeatSource, subscriber_t subscriber);

void ProtocolUnsubscribeFromHeartbeat(char *heartbeatSource, subscriber_t subscriber);

void ProtocolSubscribeForLost(char *twin, subscriber_t subscriber);

void ProtocolUnsubscribeFromLost(char *twin, subscriber_t subscriber);

/* endregion */

#endif /* ELASTIC_AI_RUNTIME_C_PROTOCOL_H */
