#include <string.h>
#include <stdlib.h>
#include "communicationEndpoint.h"
#include "ExampleLocalBroker.h"
#include "topicMatcher.h"
#include "stdio.h"

int numberSubscriber = 0;
char *identifier;
Subscription subscriberList[64];

char *concatIDWithTopic(const char *topic) {
    char *result = malloc(strlen(identifier) + strlen(topic) + 1);
    strcpy(result, identifier);
    strcat(result, topic);
    return result;
}

void publish(Posting posting) {
    publishRaw(concatIDWithTopic(posting.topic), posting);
}

void publishRaw(char *topic, Posting posting) {
    for (int i = 0; i < numberSubscriber; ++i) {
        if (checkIfTopicMatches(subscriberList[i].topic, topic)) {
            subscriberList[i].subscriber.deliver(posting);
        }
    }
    free(topic);
}

void subscribe(char *topic, Subscriber subscriber) {
    subscribeRaw(concatIDWithTopic(topic), subscriber);
}

void unsubscribe(char *topic, Subscriber subscriber) {
    unsubscribeRaw(concatIDWithTopic(topic), subscriber);
}

void subscribeRaw(char *topic, Subscriber subscriber) {
    subscriberList[numberSubscriber] = (Subscription) {.topic=topic, .subscriber=subscriber};
    numberSubscriber++;
}

void unsubscribeRaw(char *topic, Subscriber subscriber) {
    for (int i = 0; i < numberSubscriber; ++i) {
        if (strcmp(subscriberList[i].topic, topic) == 0) {
            if (subscriberList[i].subscriber.deliver == subscriber.deliver) {
                printf("EQUAL DELIVER");
                strcpy(subscriberList[i].topic, "\0");
            }
        }
    }
}

void setID(char *ID) {
    identifier = ID;
}

char *getID() {
    return identifier;
}
