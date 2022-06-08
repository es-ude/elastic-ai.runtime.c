#ifndef ELASTIC_AI_RUNTIME_C_EXAMPLELOCALBROKER_H
#define ELASTIC_AI_RUNTIME_C_EXAMPLELOCALBROKER_H

#include "subscriber.h"
#include "stdbool.h"

typedef struct Subscription {
    char *topic;
    Subscriber subscriber;
} Subscription;

char *concatIDWithTopic(const char *topic);

void setID(char *newID);

#endif //ELASTIC_AI_RUNTIME_C_EXAMPLELOCALBROKER_H
