#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "communicationEndpoint.h"
#include "Broker.h"

int numberSubscriber = 0;
char *identifier;
Subscription subscriberList[64];

char *concatIDWithTopic(const char *topic) {
    char *result = malloc(strlen(identifier) + strlen(topic) + 1);
    strcpy(result, identifier);
    strcat(result, topic);
    return result;
}

bool checkIfTopicMatches(char *subscribedTopic, char *publishedTopic) {
    if (topicsAreEqual(subscribedTopic, publishedTopic)) {
        return true;
    }
    int subscribedIterator = 0;
    int publishedIterator = 0;

    while (publishedIterator < strlen(publishedTopic)) {
        if (multilevelWildcardIn(subscribedTopic, subscribedIterator)) {
            return true;
        }
        if (checkForEqualCharacterIn(subscribedTopic, publishedTopic, subscribedIterator, publishedIterator)) {
            subscribedIterator++;
            publishedIterator++;
        } else if (checkForSingleLevelWildcardIn(subscribedTopic, subscribedIterator)) {
            publishedIterator++;
            if (checkIfWildcardEndedIn(publishedTopic, publishedIterator)) {
                subscribedIterator++;
            }
        } else {
            return false;
        }
    }
    if (subscribedTopicHasCharactersLeft(subscribedTopic, subscribedIterator)) {
        return false;
    }
    return true;
}

bool topicsAreEqual(char *subscribedTopic, char *publishedTopic) {
    return strcmp(subscribedTopic, publishedTopic) == 0;
}

bool subscribedTopicHasCharactersLeft(char *subscribedTopic, int subscribedIterator) {
    return subscribedIterator < strlen(subscribedTopic) && subscribedTopic[subscribedIterator] != '+';
}

bool multilevelWildcardIn(const char* subscribedTopic, int subscribedIterator) {
    return subscribedTopic[subscribedIterator] == '#';
}

bool checkForEqualCharacterIn(const char *subscribedTopic, const char *publishedTopic, int subscribedIterator, int publishedIterator) {
    return subscribedTopic[subscribedIterator] == publishedTopic[publishedIterator];
}

bool checkForSingleLevelWildcardIn(const char *subscribedTopic, int subscribedIterator) {
    return subscribedTopic[subscribedIterator] == '+';
}

bool checkIfWildcardEndedIn(const char *publishedTopic, int publishedIterator) {
    return publishedTopic[publishedIterator] == '/';
}

void publish(Posting posting) {
    char *topic = concatIDWithTopic(posting.topic);

    for (int i = 0; i < numberSubscriber; ++i) {
        if (checkIfTopicMatches(subscriberList[i].topic, topic)) {
            subscriberList[i].subscriber.deliver(posting);
        }
    }
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
                strcpy(subscriberList[i].topic, "\0");
            }
        }
    }
}

void setID(char *newID) {
    identifier = newID;
}

char *getID() {
    return identifier;
}
