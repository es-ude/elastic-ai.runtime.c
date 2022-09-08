#ifndef ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERNAL_H
#define ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERNAL_H

#include "Subscriber.h"

void ProtocolInternPublish(char *type, char *dataId, char *valueToPublish);

void ProtocolInternSubscribe(char *type, char *data, subscriber_t subscriber);

void ProtocolInternUnsubscribe(char *type, char *data, subscriber_t subscriber);

void ProtocolInternPublishRemote(char *twin, char *type, char *dataId, char *valueToPulish);

void ProtocolInternSubscribeRemote(char *twin, char *type, char *data, subscriber_t subscriber);

void ProtocolInternUnsubscribeRemote(char *twin, char *type, char *data, subscriber_t subscriber);

char *ProtocolInternAddType(const char *type, const char *data);

char *ProtocolInternGetTopic(const char *twin, const char *type, const char *data);

#endif /* ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERNAL_H */
