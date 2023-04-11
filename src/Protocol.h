#ifndef ELASTIC_AI_RUNTIME_C_PROTOCOL_H
#define ELASTIC_AI_RUNTIME_C_PROTOCOL_H

#include "Status.h"
#include "Subscriber.h"

#define DATA "DATA"
#define START "START"
#define STOP "STOP"
#define DO "DO"
#define DONE "DONE"
#define LOST "LOST"
#define HEARTBEAT "HEART"
#define STATUS "STATUS"

char *getStatusMessage(status_t status);

/* region SELF */

void protocolPublishData(char *dataId, char *valueToPublish);

void protocolPublishHeartbeat(char *who);

void protocolSubscribeForDataStartRequest(char *dataId, subscriber_t subscriber);

void protocolUnsubscribeFromDataStartRequest(char *dataId, subscriber_t subscriber);

void protocolSubscribeForDataStopRequest(char *dataId, subscriber_t subscriber);

void protocolUnsubscribeFromDataStopRequest(char *dataId, subscriber_t subscriber);

void protocolSubscribeForCommand(char *dataId, subscriber_t subscriber);

void protocolUnsubscribeFromCommand(char *dataId, subscriber_t subscriber);

void protocolPublishCommandResponse(char *commandId, bool commandExecutionSuccess);

void protocolPublishStatus(status_t status);

/* endregion */

/*region REMOTE */

void protocolPublishDataStartRequest(char *twin, char *dataId, char *receiver);

void protocolPublishDataStopRequest(char *twin, char *dataId, char *receiver);

void protocolPublishCommand(char *twin, char *service, char *cmd);

void protocolSubscribeForCommandResponse(char *twin, char *commandId, subscriber_t subscriber);

void protocolUnsubscribeFromCommandResponse(char *twin, char *commandId, subscriber_t subscriber);

void protocolSubscribeForData(char *twin, char *dataId, subscriber_t subscriber);

void protocolUnsubscribeFromData(char *twin, char *dataId, subscriber_t subscriber);

void protocolSubscribeForHeartbeat(char *heartbeatSource, subscriber_t subscriber);

void protocolUnsubscribeFromHeartbeat(char *heartbeatSource, subscriber_t subscriber);

void protocolSubscribeForLost(char *twin, subscriber_t subscriber);

void protocolUnsubscribeFromLost(char *twin, subscriber_t subscriber);

void protocolSubscribeForStatus(char *twin, subscriber_t subscriber);

void protocolUnsubscribeFromStatus(char *twin, subscriber_t subscriber);

/* endregion */

#endif /* ELASTIC_AI_RUNTIME_C_PROTOCOL_H */
