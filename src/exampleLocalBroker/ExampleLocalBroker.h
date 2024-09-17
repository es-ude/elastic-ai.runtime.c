#ifndef ELASTIC_AI_RUNTIME_C_EXAMPLE_LOCAL_BROKER_H
#define ELASTIC_AI_RUNTIME_C_EXAMPLE_LOCAL_BROKER_H

#include "Subscriber.h"
#include <stdbool.h>

/*! @brief Initializes the Broker
 *
 * Sets the Domain and device identifier, needs to be called before any other function of the Broker
 * is called.
 *
 * @param domain Domain of the Broker
 * @param deviceIdentifier Unique identifier of the device in the MQTT network
 */
void init(char *domain, char *deviceIdentifier);

typedef struct subscription subscription_t;
struct subscription {
    char *topic;
    subscriber_t subscriber;
    subscription_t *next;
};

#endif /* ELASTIC_AI_RUNTIME_C_EXAMPLE_LOCAL_BROKER_H */
