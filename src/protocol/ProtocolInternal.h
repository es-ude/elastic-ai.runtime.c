#ifndef ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERNAL_H
#define ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERNAL_H

#include "Subscriber.h"

static void protocolInternPublish(char *type, char *dataId, char *valueToPublish, bool retain);

static void protocolInternSubscribe(char *type, char *data, subscriber_t subscriber);

static void protocolInternUnsubscribe(char *type, char *data, subscriber_t subscriber);

static void protocolInternPublishRemote(char *twin, char *type, char *dataId, char *valueToPublish);

static void protocolInternSubscribeRemote(char *twin, char *type, char *data,
                                          subscriber_t subscriber);

static void protocolInternUnsubscribeRemote(char *twin, char *type, char *data,
                                            subscriber_t subscriber);

static char *protocolInternAddType(const char *type, const char *data);

static char *protocolInternGetTopic(const char *twin, const char *type, const char *data);

#endif /* ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERNAL_H */
