#ifndef ELASTIC_AI_RUNTIME_C_BROKER_H
#define ELASTIC_AI_RUNTIME_C_BROKER_H

#include <stdint.h>

typedef struct subscription {
    char topic[26];
    void (*callback_fct)(char *topic, uint32_t topic_length, uint32_t data);
} Subscription_t;

void subscribe(Subscription_t subscription);
void publish(char* topic, uint32_t topic_length, uint32_t data);

#endif //ELASTIC_AI_RUNTIME_C_BROKER_H
