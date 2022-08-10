#ifndef ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERN_H
#define ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERN_H

#include "subscriber.h"

void publish_intern(char *type, char *dataId, char *value);

void subscribe_intern(char *type, char *data, Subscriber subscriber);

void unsubscribe_intern(char *type, char *data, Subscriber subscriber);

void publishRemote_intern(char *twin, char *type, char *dataId, char *value);

void subscribeRemote_intern(char *twin, char *type, char *data, Subscriber subscriber);

void unsubscribeRemote_intern(char *twin, char *type, char *data, Subscriber subscriber);

char *addType(const char *type, const char *data);

char *getTopic(const char *twin, const char *type, const char *data);

#endif // ELASTIC_AI_RUNTIME_C_PROTOCOL_INTERN_H
