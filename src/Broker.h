#ifndef ELASTIC_AI_RUNTIME_C_BROKER_H
#define ELASTIC_AI_RUNTIME_C_BROKER_H

#include "subscriber.h"

typedef struct Subscription {
    char *topic;
    Subscriber subscriber;
} Subscription;

#endif //ELASTIC_AI_RUNTIME_C_BROKER_H
