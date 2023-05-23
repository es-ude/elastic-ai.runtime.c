#ifndef ELASTIC_AI_RUNTIME_C_SUBSCRIBER_H
#define ELASTIC_AI_RUNTIME_C_SUBSCRIBER_H

#include "Posting.h"

/*!
 * Struct representing a subscriber
 */
struct subscriber {
    /*! The function called with the posting when a message is published to it */
    void (*deliver)(posting_t posting);
};
typedef struct subscriber subscriber_t;

#endif /* ELASTIC_AI_RUNTIME_C_SUBSCRIBER_H */
