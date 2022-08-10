#ifndef ELASTIC_AI_RUNTIME_C_EXAMPLELOCALBROKER_H
#define ELASTIC_AI_RUNTIME_C_EXAMPLELOCALBROKER_H

#include "stdbool.h"
#include "subscriber.h"

/*! @brief Initializes the Broker
 *
 * Sets the Domain and device identifier, needs to be called before any other function of the Broker
 * is called.
 *
 * @param domain Domain of the Broker
 * @param deviceIdentifier Unique identifier of the device in the MQTT network
 */
void init(char *domain, char *deviceIdentifier);

typedef struct Subscription {
    char *topic;
    Subscriber subscriber;
} Subscription;

#endif // ELASTIC_AI_RUNTIME_C_EXAMPLELOCALBROKER_H
