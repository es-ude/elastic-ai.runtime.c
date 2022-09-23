#ifndef ELASTIC_AI_RUNTIME_C_TOPICMATCHER_H
#define ELASTIC_AI_RUNTIME_C_TOPICMATCHER_H

#include <stdbool.h>

/*! Checks if published topic should be delivered to the subscribed topic
 *
 * @param subscribedTopic Topic that is subscribed
 * @param publishedTopic Topic that is published
 * @return True if topic matches
 */
bool topicMatcherCheckIfTopicMatches(char *subscribedTopic, char *publishedTopic);

#endif /* ELASTIC_AI_RUNTIME_C_TOPICMATCHER_H */
