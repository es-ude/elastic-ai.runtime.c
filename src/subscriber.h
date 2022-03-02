#ifndef ELASTIC_AI_RUNTIME_C_SUBSCRIBER_H
#define ELASTIC_AI_RUNTIME_C_SUBSCRIBER_H

#include "posting.h"

typedef struct Subscriber {
    void (*deliver)(Posting posting);
} Subscriber;

#endif //ELASTIC_AI_RUNTIME_C_SUBSCRIBER_H
