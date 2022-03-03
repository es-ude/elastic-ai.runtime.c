#ifndef ELASTIC_AI_RUNTIME_C_POSTING_H
#define ELASTIC_AI_RUNTIME_C_POSTING_H

typedef struct Posting {
    char *topic;
    char *data;
} Posting;

char *addType(const char *type, const char *topic);

Posting createTurnOn(char *topic);

Posting createTurnOff(char *topic);

Posting createCommand(char *topic, char *command);

Posting createStartSending(char *dataId, char *receiver);

Posting createStopSending(char *dataId, char *receiver);

Posting createData(char *phenomena, char *value);

Posting createHeartbeat(char *heartbeatSource);

#endif //ELASTIC_AI_RUNTIME_C_POSTING_H
