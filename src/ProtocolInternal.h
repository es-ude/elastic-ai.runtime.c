
#ifndef INCLUDE_PROTOCOL_INTERNAL
#warning Internal header file "ProtocolInternal.h" included from elsewhere
#undef INCLUDE_PROTOCOL_INTERNAL
#endif

#ifndef ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERNAL_H
#define ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERNAL_H

#include "Subscriber.h"

void protocolInternPublish(char *type, char *dataId, char *valueToPublish, bool retain);

void protocolInternSubscribe(char *type, char *data, subscriber_t subscriber);

void protocolInternUnsubscribe(char *type, char *data, subscriber_t subscriber);

void protocolInternPublishRemote(char *twin, char *type, char *dataId, char *valueToPublish);

void protocolInternSubscribeRemote(char *twin, char *type, char *data, subscriber_t subscriber);

void protocolInternUnsubscribeRemote(char *twin, char *type, char *data, subscriber_t subscriber);

char *protocolInternAddType(const char *type, const char *data);

char *protocolInternGetTopic(const char *twin, const char *type, const char *data);

#endif /* ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERNAL_H */
