#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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

bool checkIfTopicMatches(char *subscribedTopic, char *publishedTopic) {
    // Equal without wildcards
    if (strcmp(subscribedTopic, publishedTopic) == 0) {
        return true;
    }
    int subscribedIterator = 0;
    int publishedIterator = 0;
    // Iterates over both strings
    while (publishedIterator < strlen(publishedTopic)) {
        // Multilevel wildcard in subscribed topic reached
        if (subscribedTopic[subscribedIterator] == '#') {
            return true;
        }
        // If both characters are equal increase iterator for both
        if (subscribedTopic[subscribedIterator] == publishedTopic[publishedIterator]) {
            subscribedIterator++;
            publishedIterator++;
            // Check for single level wildcard if characters were not equal
        } else if (subscribedTopic[subscribedIterator] == '+') {
            publishedIterator++;
            // When the published topic reaches slash, the single level wildcard is over
            if (publishedTopic[publishedIterator] == '/') {
                subscribedIterator++;
            }
        } else {
            return false;
        }
    }
    // If the subscribed topic has characters left
    if (subscribedIterator < strlen(subscribedTopic) && subscribedTopic[subscribedIterator] != '+') {
        return false;
    }
    return true;
}

void publish(Posting posting) {
    char *topic = addID(posting.topic);

    for (int i = 0; i < numberSubscriber; ++i) {
        if (checkIfTopicMatches(subscriberList[i].topic, topic)) {
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
                strcpy(subscriberList[i].topic, "\0");
            }
        }
    }
}

char *ID() {
    return identifier;
}
