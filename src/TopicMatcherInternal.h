#ifndef ELASTIC_AI_RUNTIME_C_TOPICMATCHER_INTERNAL_H
#define ELASTIC_AI_RUNTIME_C_TOPICMATCHER_INTERNAL_H

static bool TopicMatcherInternalTopicsAreEqual(char *subscribedTopic, char *publishedTopic);

static bool TopicMatcherInternalSubscribedTopicHasCharactersLeft(char *subscribedTopic,
                                                                 int subscribedIterator);

static bool TopicMatcherInternalMultiLevelWildcardIn(const char *subscribedTopic,
                                                     int subscribedIterator);

static bool TopicMatcherInternalCheckForEqualCharacterIn(const char *subscribedTopic,
                                                         const char *publishedTopic,
                                                         int subscribedIterator,
                                                         int publishedIterator);

static bool TopicMatcherInternalCheckForSingleLevelWildcardIn(const char *subscribedTopic,
                                                              int subscribedIterator);

static bool TopicMatcherInternalCheckIfWildcardEndedIn(const char *publishedTopic,
                                                       int publishedIterator);

#endif /* ELASTIC_AI_RUNTIME_C_TOPICMATCHER_INTERNAL_H */
