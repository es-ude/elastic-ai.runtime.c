#ifndef ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H
#define ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H

#include "posting.h"
#include "subscriber.h"

/*!
 * A MQTT-Broker implementation needs to implements these functions.
 */

void init(char *Domain, char *ID);

void setDomain(char *Domain);

char *getDomain();

void setDeviceID(char *ID);

char *getDeviceID();

void publish(Posting posting);

void publishRemote(Posting posting);

void publishRaw(Posting posting);

void subscribe(char *topic, Subscriber subscriber);

void subscribeRemote(char *topic, Subscriber subscriber);

void unsubscribe(char *topic, Subscriber subscriber);

void subscribeRaw(char *topic, Subscriber subscriber);

void unsubscribeRemote(char *topic, Subscriber subscriber);

void unsubscribeRaw(char *topic, Subscriber subscriber);

#endif // ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H
