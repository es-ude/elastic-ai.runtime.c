#ifndef ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H
#define ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H

#include "posting.h"
#include "subscriber.h"

void publish(Posting posting);

void subscribe(char *topic, Subscriber subscriber);

void unsubscribe(char *topic, Subscriber subscriber);

void subscribeRaw(char *topic, Subscriber subscriber);

void unsubscribeRaw(char *topic, Subscriber subscriber);

char *ID();

#endif //ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H
