#ifndef ELASTIC_AI_RUNTIME_C_TOPICMATCHER_INTERNAL_H
#define ELASTIC_AI_RUNTIME_C_TOPICMATCHER_INTERNAL_H

static bool topicMatcherInternalTopicsAreEqual(char *subscribedTopic, char *publishedTopic);

static bool topicMatcherInternalSubscribedTopicHasCharactersLeft(char *subscribedTopic,
                                                                 int subscribedIterator);

static bool topicMatcherInternalMultiLevelWildcardIn(const char *subscribedTopic,
                                                     int subscribedIterator);

static bool topicMatcherInternalCheckForEqualCharacterIn(const char *subscribedTopic,
                                                         const char *publishedTopic,
                                                         int subscribedIterator,
                                                         int publishedIterator);

static bool topicMatcherInternalCheckForSingleLevelWildcardIn(const char *subscribedTopic,
                                                              int subscribedIterator);

static bool topicMatcherInternalCheckIfWildcardEndedIn(const char *publishedTopic,
                                                       int publishedIterator);

#endif /* ELASTIC_AI_RUNTIME_C_TOPICMATCHER_INTERNAL_H */
