#ifndef ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H
#define ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H

#include "posting.h"
#include "subscriber.h"

/*
 A MQTT Broker needs to implement these functions
 */

void publish(Posting posting);

void publishRaw(char *topic, Posting posting);

void subscribe(char *topic, Subscriber subscriber);

void unsubscribe(char *topic, Subscriber subscriber);

void subscribeRaw(char *topic, Subscriber subscriber);

void unsubscribeRaw(char *topic, Subscriber subscriber);

char *getID();

void setID(char *ID);

#endif // ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H
