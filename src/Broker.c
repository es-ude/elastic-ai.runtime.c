#include <stdint.h>
#include "Broker.h"

int numberSubscribe = 0;

Subscription_t subscriberList[10];

void subscribe(Subscription_t subscription) {
    subscriberList[numberSubscribe] = subscription;
    numberSubscribe++;
}

void publish(char *topic, uint32_t topic_length, uint32_t data) {
    subscriberList[0].callback_fct(topic, topic_length, 4);
}
