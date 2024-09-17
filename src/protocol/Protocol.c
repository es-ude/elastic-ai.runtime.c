#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CommunicationEndpoint.h"
#include "Posting.h"
#include "Protocol.h"
#include "ProtocolInternal.h"

static char *addToStatus(char *current, char *descriptor, char *value, bool mandatory) {
    if (!mandatory) {
        if (value == NULL) {
            return current;
        }
        if (strlen(value) == 0) {
            return current;
        }
    }

    char *updatedStatus;
    if (current != NULL) {
        if (value == NULL || strlen(value) == 0) {
            updatedStatus =
                calloc(strlen(current) + strlen(descriptor) + strlen("NULL") + 3, sizeof(char));
            sprintf(updatedStatus, "%s%s:%s;", current, descriptor, "NULL");
        } else {
            updatedStatus =
                calloc(strlen(current) + strlen(descriptor) + strlen(value) + 3, sizeof(char));
            sprintf(updatedStatus, "%s%s:%s;", current, descriptor, value);
        }
        free(current);
    } else {
        if (value == NULL || strlen(value) == 0) {
            updatedStatus = calloc(strlen(descriptor) + strlen("NULL") + 3, sizeof(char));
            sprintf(updatedStatus, "%s:%s;", descriptor, "NULL");
        } else {
            updatedStatus = calloc(strlen(descriptor) + strlen(value) + 3, sizeof(char));
            sprintf(updatedStatus, "%s:%s;", descriptor, value);
        }
    }

    return updatedStatus;
}

char *getStatusMessage(status_t status) {
    char *statusMessage = addToStatus(NULL, STATUS_ID, status.id, true);
    statusMessage = addToStatus(statusMessage, STATUS_TYPE, status.type, true);
    statusMessage = addToStatus(statusMessage, STATUS_STATE, status.state, true);

    statusMessage = addToStatus(statusMessage, STATUS_DATA, status.data, false);
    statusMessage = addToStatus(statusMessage, STATUS_FPGA, status.fpga, false);
    statusMessage = addToStatus(statusMessage, STATUS_VERSION, status.version, false);
    statusMessage =
        addToStatus(statusMessage, STATUS_APPLICATIONS, status.storedApplications, false);

    return statusMessage;
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
    protocolInternSubscribe(DO, dataId, subscriber);
}

void protocolUnsubscribeFromCommand(char *dataId, subscriber_t subscriber) {
    protocolInternUnsubscribe(DO, dataId, subscriber);
}

void protocolPublishCommandResponse(char *commandId, bool commandExecutionSuccess) {
    protocolInternPublish(DONE, commandId, commandExecutionSuccess ? "SUCCESS" : "FAIL", false);
}

void protocolPublishStatus(status_t status) {
    char *statusMessage = getStatusMessage(status);
    protocolInternPublish(STATUS, "", statusMessage, true);
    free(statusMessage);
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
    protocolInternPublishRemote(twin, DO, service, cmd);
}

void protocolSubscribeForCommandResponse(char *twin, char *commandId, subscriber_t subscriber) {
    protocolInternSubscribeRemote(twin, DONE, commandId, subscriber);
}

void protocolUnsubscribeFromCommandResponse(char *twin, char *commandId, subscriber_t subscriber) {
    protocolInternUnsubscribeRemote(twin, DONE, commandId, subscriber);
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
    posting_t posting = {.topic = topic, .data = valueToPublish, .retain = retain};
    communicationEndpointPublish(posting);
    free(topic);
}

void protocolInternSubscribe(char *type, char *data, subscriber_t subscriber) {
    char *result = protocolInternAddType(type, data);
    communicationEndpointSubscribe(result, subscriber);
    free(result);
}

void protocolInternUnsubscribe(char *type, char *data, subscriber_t subscriber) {
    char *result = protocolInternAddType(type, data);
    communicationEndpointUnsubscribe(result, subscriber);
    free(result);
}

char *protocolInternGetTopic(const char *twin, const char *type, const char *data) {
    size_t length = strlen(twin) + strlen(type) + strlen(data) + 3;
    char *result = calloc(length, sizeof(char));
    if (strlen(data) == 0) {
        snprintf(result, length, "%s/%s", twin, type);
    } else {
        snprintf(result, length, "%s/%s/%s", twin, type, data);
    }

    return result;
}

char *protocolInternAddType(const char *type, const char *data) {
    char *fixedTopic = calloc(strlen(type) + strlen(data) + 2, sizeof(char));
    if (strlen(data) == 0) {
        sprintf(fixedTopic, "%s", type);
    } else {
        sprintf(fixedTopic, "%s/%s", type, data);
    }
    return fixedTopic;
}

/* endregion INTERNAL HEADER FUNCTIONS */
