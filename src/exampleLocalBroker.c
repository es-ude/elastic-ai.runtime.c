#include "exampleLocalBroker.h"
#include "communicationEndpoint.h"
#include "topicMatcher.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numberSubscriber = 0;
char *brokerDomain;
char *brokerDeviceIdentifier;
Subscription subscriberList[64];

void init(char *domain, char *deviceIdentifier) {
    brokerDomain = malloc(strlen(domain) + 1);
    brokerDomain = domain;
    brokerDeviceIdentifier = malloc(strlen(deviceIdentifier) + 1);
    brokerDeviceIdentifier = deviceIdentifier;
}

void setDomain(char *deviceIdentifier) {
    free(brokerDomain);
    brokerDomain = malloc(strlen(deviceIdentifier) + 1);
    brokerDomain = deviceIdentifier;
}

char *getDomain() {
    return brokerDomain;
}

void setDeviceID(char *deviceIdentifier) {
    free(brokerDeviceIdentifier);
    brokerDeviceIdentifier = malloc(strlen(deviceIdentifier) + 1);
    brokerDeviceIdentifier = deviceIdentifier;
}

char *getDeviceID() {
    return brokerDeviceIdentifier;
}

char *concatIDWithDomain(const char *topic) {
    uint16_t length = strlen(brokerDomain) + strlen(topic) + 2;
    char *result = malloc(length);
    snprintf(result, length, "%s/%s", brokerDomain, topic);
    return result;
}

char *concatIDWithDomainAndID(const char *topic) {
    uint16_t length = strlen(brokerDomain) + strlen(brokerDeviceIdentifier) + strlen(topic) + 3;
    char *result = malloc(length);
    snprintf(result, length, "%s/%s/%s", brokerDomain, brokerDeviceIdentifier, topic);
    return result;
}

void publish(Posting posting) {
    posting.topic = concatIDWithDomainAndID(posting.topic);
    publishRaw(posting);
    free(posting.topic);
}

void publishRemote(Posting posting) {
    posting.topic = concatIDWithDomain(posting.topic);
    publishRaw(posting);
    free(posting.topic);
}

void publishRaw(Posting posting) {
    for (int i = 0; i < numberSubscriber; ++i) {
        if (checkIfTopicMatches(subscriberList[i].topic, posting.topic)) {
            subscriberList[i].subscriber.deliver(posting);
        }
    }
    printf("Published to:\t\t%s\n", posting.topic);
}

void subscribe(char *topic, Subscriber subscriber) {
    char *newTopic = concatIDWithDomainAndID(topic);
    subscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void subscribeRemote(char *topic, Subscriber subscriber) {
    char *newTopic = concatIDWithDomain(topic);
    subscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void unsubscribe(char *topic, Subscriber subscriber) {
    char *newTopic = concatIDWithDomainAndID(topic);
    unsubscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void unsubscribeRemote(char *topic, Subscriber subscriber) {
    char *newTopic = concatIDWithDomain(topic);
    unsubscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void subscribeRaw(char *topic, Subscriber subscriber) {
    char *newTopic = malloc(strlen(topic) + 1);
    strcpy(newTopic, topic);
    subscriberList[numberSubscriber] = (Subscription){.topic = newTopic, .subscriber = subscriber};
    numberSubscriber++;
    printf("Subscribed to:\t\t%s\n", topic);
}

void unsubscribeRaw(char *topic, Subscriber subscriber) {
    for (int i = 0; i < numberSubscriber; ++i) {
        if (strcmp(subscriberList[i].topic, topic) == 0) {
            if (subscriberList[i].subscriber.deliver == subscriber.deliver) {
                strcpy(subscriberList[i].topic, "\0");
                //                free(subscriberList[i].topic);
            }
            printf("Unsubscribed from:\t%s\n", topic);
        }
    }
}
