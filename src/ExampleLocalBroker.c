#include "ExampleLocalBroker.h"
#include "CommunicationEndpoint.h"
#include "TopicMatcher.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numberSubscriber = 0;
char *brokerDomain;
char *brokerDeviceIdentifier;
subscription_t subscriberList[64];

void init(char *domain, char *deviceIdentifier) {
    brokerDomain = malloc(strlen(domain) + 1);
    brokerDomain = domain;
    brokerDeviceIdentifier = malloc(strlen(deviceIdentifier) + 1);
    brokerDeviceIdentifier = deviceIdentifier;
}

void CommunicationEndpointSetDomain(char *deviceIdentifier) {
    free(brokerDomain);
    brokerDomain = malloc(strlen(deviceIdentifier) + 1);
    brokerDomain = deviceIdentifier;
}

char *CommunicationEndpointGetDomain() {
    return brokerDomain;
}

void CommunicationEndpointSetDeviceId(char *deviceIdentifier) {
    free(brokerDeviceIdentifier);
    brokerDeviceIdentifier = malloc(strlen(deviceIdentifier) + 1);
    brokerDeviceIdentifier = deviceIdentifier;
}

char *CommunicationEndpointGetDeviceId() {
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

void CommunicationEndpointPublish(posting_t posting) {
    posting.topic = concatIDWithDomainAndID(posting.topic);
    CommunicationEndpointPublishRaw(posting);
    free(posting.topic);
}

void CommunicationEndpointPublishRemote(posting_t posting) {
    posting.topic = concatIDWithDomain(posting.topic);
    CommunicationEndpointPublishRaw(posting);
    free(posting.topic);
}

void CommunicationEndpointPublishRaw(posting_t posting) {
    for (int i = 0; i < numberSubscriber; ++i) {
        if (TopicMatcherCheckIfTopicMatches(subscriberList[i].topic, posting.topic)) {
            subscriberList[i].subscriber.deliver(posting);
        }
    }
    printf("Published to:\t\t%s\n", posting.topic);
}

void CommunicationEndpointSubscribe(char *topic, subscriber_t subscriber) {
    char *newTopic = concatIDWithDomainAndID(topic);
    CommunicationEndpointSubscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void CommunicationEndpointSubscribeRemote(char *topic, subscriber_t subscriber) {
    char *newTopic = concatIDWithDomain(topic);
    CommunicationEndpointSubscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void CommunicationEndpointUnsubscribe(char *topic, subscriber_t subscriber) {
    char *newTopic = concatIDWithDomainAndID(topic);
    CommunicationEndpointUnsubscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void CommunicationEndpointUnsubscribeRemote(char *topic, subscriber_t subscriber) {
    char *newTopic = concatIDWithDomain(topic);
    CommunicationEndpointUnsubscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void CommunicationEndpointSubscribeRaw(char *topic, subscriber_t subscriber) {
    char *newTopic = malloc(strlen(topic) + 1);
    strcpy(newTopic, topic);
    subscriberList[numberSubscriber] =
        (subscription_t){.topic = newTopic, .subscriber = subscriber};
    numberSubscriber++;
    printf("Subscribed to:\t\t%s\n", topic);
}

void CommunicationEndpointUnsubscribeRaw(char *topic, subscriber_t subscriber) {
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
