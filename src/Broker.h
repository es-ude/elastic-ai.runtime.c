#ifndef ELASTIC_AI_RUNTIME_C_BROKER_H
#define ELASTIC_AI_RUNTIME_C_BROKER_H

#include "subscriber.h"

typedef struct Subscription {
    char *topic;
    Subscriber subscriber;
} Subscription;

char *addID(const char *topic);
bool checkIfTopicMatches(char *subscribedTopic, char *publishedTopic);
bool topicsAreEqual(char *subscribedTopic, char *publishedTopic);
bool subscribedTopicHasCharactersLeft(char *subscribedTopic, int subscribedIterator);
bool multilevelWildcardIn(const char* subscribedTopic, int subscribedIterator);
bool checkForEqualCharacterIn(const char *subscribedTopic, const char *publishedTopic, int subscribedIterator, int publishedIterator);
bool checkForSingleLevelWildcardIn(const char *subscribedTopic, int subscribedIterator);
bool checkIfWildcardEndedIn(const char *publishedTopic, int publishedIterator);
char *ID();

#endif //ELASTIC_AI_RUNTIME_C_BROKER_H
