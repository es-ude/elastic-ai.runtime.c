#include <stdint.h>

typedef struct subscription {
    char topic[15];
    void (*callback_fct)(char *topic, uint32_t data);
}Subscription_t;

int numberSubscribe=0;

Subscription_t subscriberList[10];
void subscribe(Subscription_t subscription) {
subscriberList[numberSubscribe];
numberSubscribe++;
}

