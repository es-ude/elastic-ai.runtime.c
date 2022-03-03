#include <string.h>
#include <stdlib.h>
#include "communicationEndpoint.h"
#include "Broker.h"

int numberSubscriber = 0;
char identifier[] = "eip://uni-due.de/es/";

Subscription subscriberList[64];

char *addID(const char *topic) {
    char *result = malloc(strlen(identifier) + strlen(topic) + 1);
    strcpy(result, identifier);
    strcat(result, topic);
    return result;
}

void publish(Posting posting) {
    char *topic = addID(posting.topic);

    for (int i = 0; i < numberSubscriber; ++i) {
        if (strcmp(subscriberList[i].topic, topic) == 0) {
            subscriberList[i].subscriber.deliver(posting);
        }
    }
}

void subscribe(char *topic, Subscriber subscriber) {
    subscribeRaw(addID(topic), subscriber);
}

void unsubscribe(char *topic, Subscriber subscriber) {
    unsubscribeRaw(addID(topic), subscriber);
}

void subscribeRaw(char *topic, Subscriber subscriber) {
    subscriberList[numberSubscriber] = (Subscription) {.topic=topic, .subscriber=subscriber};
    numberSubscriber++;
}

void unsubscribeRaw(char *topic, Subscriber subscriber) {
    for (int i = 0; i < numberSubscriber; ++i) {
        if (strcmp(subscriberList[i].topic, topic) == 0) {
            if (subscriberList[i].subscriber.deliver == subscriber.deliver) {
                //subscriberList[i].subscriber = (Subscriber) {.deliver=  };
                strcpy(subscriberList[i].topic, "\0");
            }
        }
    }
}

char *ID() {
    return identifier;
}
