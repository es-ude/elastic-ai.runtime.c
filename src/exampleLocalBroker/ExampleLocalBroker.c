#include "ExampleLocalBroker.h"
#include "CommunicationEndpoint.h"
#include "TopicMatcher.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *brokerDomain;
char *brokerDeviceIdentifier;
subscription_t *subscriberList = NULL;

void init(char *domain, char *deviceIdentifier) {
    brokerDomain = calloc(strlen(domain) + 1, sizeof(char));
    strcpy(brokerDomain, domain);

    brokerDeviceIdentifier = calloc(strlen(deviceIdentifier) + 1, sizeof(char));
    strcpy(brokerDeviceIdentifier, deviceIdentifier);

    while (subscriberList != NULL) {
        subscription_t *current = subscriberList;
        subscriberList = current->next;
        free(current->topic);
        free(current);
    }
}

void communicationEndpointSetBrokerDomain(char *domain) {
    free(brokerDomain);
    brokerDomain = calloc(strlen(domain) + 1, sizeof(char));
    strcpy(brokerDomain, domain);
}

char *communicationEndpointGetBrokerDomain() {
    return brokerDomain;
}

uint8_t communicationEndpointSetUserConfiguration(char *clientId, char *userId, char *password) {
    free(brokerDeviceIdentifier);
    brokerDeviceIdentifier = calloc(strlen(clientId) + 1, sizeof(char));
    strcpy(brokerDeviceIdentifier, clientId);
    return 0x00;
}

char *communicationEndpointGetClientId() {
    return brokerDeviceIdentifier;
}

char *concatIdWithDomain(const char *topic) {
    size_t length = strlen(brokerDomain) + strlen(topic) + 2;
    char *result = calloc(length, sizeof(char));
    snprintf(result, length, "%s/%s", brokerDomain, topic);
    return result;
}

char *concatIdWithDomainAndTopic(const char *topic) {
    size_t length = strlen(brokerDomain) + strlen(brokerDeviceIdentifier) + strlen(topic) + 3;
    char *result = calloc(length, sizeof(char));
    snprintf(result, length, "%s/%s/%s", brokerDomain, brokerDeviceIdentifier, topic);
    return result;
}

void communicationEndpointPublish(posting_t posting) {
    posting_t post = {
        .topic = concatIdWithDomainAndTopic(posting.topic),
        .data = posting.data,
        .retain = posting.retain,
    };
    communicationEndpointPublishRaw(post);
    free(post.topic);
}

void communicationEndpointPublishRemote(posting_t posting) {
    posting_t post = {
        .topic = concatIdWithDomain(posting.topic), .data = posting.data, .retain = posting.retain};
    communicationEndpointPublishRaw(post);
    free(post.topic);
}

void communicationEndpointPublishRaw(posting_t posting) {
    subscription_t *current = subscriberList;
    while (current != NULL) {
        if (topicMatcherCheckIfTopicMatches(current->topic, posting.topic)) {
            current->subscriber.deliver(posting);
        }
        current = current->next;
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

void communicationEndpointSubscribeRaw(char *topic, subscriber_t subscriber) {
    subscription_t *newSubscription = calloc(1, sizeof(subscription_t));
    newSubscription->topic = calloc(strlen(topic) + 1, sizeof(char));
    strcpy(newSubscription->topic, topic);
    newSubscription->subscriber = subscriber;

    if (subscriberList == NULL) {
        subscriberList = newSubscription;
    } else {
        subscription_t *current = subscriberList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newSubscription;
    }

    printf("Subscribed to:\t\t%s\n", topic);
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

void communicationEndpointUnsubscribeRaw(char *topic,
                                         __attribute__((unused)) subscriber_t subscriber) {
    if (subscriberList == NULL) {
        return;
    }

    subscription_t *current = subscriberList;
    subscription_t *last = NULL;
    while (current != NULL) {
        if (topicMatcherCheckIfTopicMatches(current->topic, topic)) {
            if (last == NULL) {
                free(current->topic);
                free(current);
                subscriberList = NULL;
            } else {
                last->next = current->next;
                free(current->topic);
                free(current);
            }
            break;
        }
        last = current;
        current = current->next;
    }
    printf("Unsubscribed from:\t%s\n", topic);
}
