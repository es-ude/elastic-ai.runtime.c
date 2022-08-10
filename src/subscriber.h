#ifndef ELASTIC_AI_RUNTIME_C_SUBSCRIBER_H
#define ELASTIC_AI_RUNTIME_C_SUBSCRIBER_H

#include "posting.h"

/*!
 * Struct representing a subscriber
 */
typedef struct Subscriber_t {
    /*! The function called with the Posting when a message is published to it */
    void (*deliver)(Posting posting);
} Subscriber;

#endif // ELASTIC_AI_RUNTIME_C_SUBSCRIBER_H
