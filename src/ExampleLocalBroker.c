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

void communicationEndpointSetBrokerDomain(char *domain) {
    free(brokerDomain);
    brokerDomain = malloc(strlen(domain) + 1);
    brokerDomain = domain;
}

char *communicationEndpointGetBrokerDomain() {
    return brokerDomain;
}

uint8_t communicationEndpointSetUserConfiguration(char *clientId, char *userId, char *password) {
    free(brokerDeviceIdentifier);
    brokerDeviceIdentifier = malloc(strlen(clientId) + 1);
    brokerDeviceIdentifier = clientId;
    return 0x00;
}

char *communicationEndpointGetClientId() {
    return brokerDeviceIdentifier;
}

char *concatIdWithDomain(const char *topic) {
    size_t length = strlen(brokerDomain) + strlen(topic) + 2;
    char *result = malloc(length);
    snprintf(result, length, "%s/%s", brokerDomain, topic);
    return result;
}

char *concatIdWithDomainAndTopic(const char *topic) {
    size_t length = strlen(brokerDomain) + strlen(brokerDeviceIdentifier) + strlen(topic) + 3;
    char *result = malloc(length);
    snprintf(result, length, "%s/%s/%s", brokerDomain, brokerDeviceIdentifier, topic);
    return result;
}

void communicationEndpointPublish(posting_t posting) {
    posting.topic = concatIdWithDomainAndTopic(posting.topic);
    communicationEndpointPublishRaw(posting);
    free(posting.topic);
}

void communicationEndpointPublishRemote(posting_t posting) {
    posting.topic = concatIdWithDomain(posting.topic);
    communicationEndpointPublishRaw(posting);
    free(posting.topic);
}

void communicationEndpointPublishRaw(posting_t posting) {
    for (int i = 0; i < numberSubscriber; ++i) {
        if (topicMatcherCheckIfTopicMatches(subscriberList[i].topic, posting.topic)) {
            subscriberList[i].subscriber.deliver(posting);
        }
    }
    printf("Published to:\t\t%s\n", posting.topic);
}

void communicationEndpointSubscribe(char *topic, subscriber_t subscriber) {
    char *newTopic = concatIdWithDomainAndTopic(topic);
    communicationEndpointSubscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void communicationEndpointSubscribeRemote(char *topic, subscriber_t subscriber) {
    char *newTopic = concatIdWithDomain(topic);
    communicationEndpointSubscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void communicationEndpointUnsubscribe(char *topic, subscriber_t subscriber) {
    char *newTopic = concatIdWithDomainAndTopic(topic);
    communicationEndpointUnsubscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void communicationEndpointUnsubscribeRemote(char *topic, subscriber_t subscriber) {
    char *newTopic = concatIdWithDomain(topic);
    communicationEndpointUnsubscribeRaw(newTopic, subscriber);
    free(newTopic);
}

void communicationEndpointSubscribeRaw(char *topic, subscriber_t subscriber) {
    char *newTopic = malloc(strlen(topic) + 1);
    strcpy(newTopic, topic);
    subscriberList[numberSubscriber] =
        (subscription_t){.topic = newTopic, .subscriber = subscriber};
    numberSubscriber++;
    printf("Subscribed to:\t\t%s\n", topic);
}

void communicationEndpointUnsubscribeRaw(char *topic, subscriber_t subscriber) {
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
