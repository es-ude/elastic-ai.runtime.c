#include "TopicMatcher.h"
#include "TopicMatcherInternal.h"
#include <stdbool.h>
#include <string.h>

/* region PUBLIC HEADER FUNCTIONS */

bool topicMatcherCheckIfTopicMatches(char *subscribedTopic, char *publishedTopic) {
    if (topicMatcherInternalTopicsAreEqual(subscribedTopic, publishedTopic)) {
        return true;
    }
    int subscribedIterator = 0;
    int publishedIterator = 0;

    while (publishedIterator < strlen(publishedTopic)) {
        if (topicMatcherInternalMultiLevelWildcardIn(subscribedTopic, subscribedIterator)) {
            return true;
        }
        if (topicMatcherInternalCheckForEqualCharacterIn(subscribedTopic, publishedTopic,
                                                         subscribedIterator, publishedIterator)) {
            subscribedIterator++;
            publishedIterator++;
        } else if (topicMatcherInternalCheckForSingleLevelWildcardIn(subscribedTopic,
                                                                     subscribedIterator)) {
            publishedIterator++;
            if (topicMatcherInternalCheckIfWildcardEndedIn(publishedTopic, publishedIterator)) {
                subscribedIterator++;
            }
        } else {
            return false;
        }
    }
    if (topicMatcherInternalSubscribedTopicHasCharactersLeft(subscribedTopic, subscribedIterator)) {
        return false;
    }
    return true;
}

/* endregion */

/* region INTERNAL HEADER FUNCTIONS */

static bool topicMatcherInternalTopicsAreEqual(char *subscribedTopic, char *publishedTopic) {
    return strcmp(subscribedTopic, publishedTopic) == 0;
}

static bool topicMatcherInternalSubscribedTopicHasCharactersLeft(char *subscribedTopic,
                                                                 int subscribedIterator) {
    return subscribedIterator < strlen(subscribedTopic) &&
           subscribedTopic[subscribedIterator] != '+';
}

static bool topicMatcherInternalMultiLevelWildcardIn(const char *subscribedTopic,
                                                     int subscribedIterator) {
    return subscribedTopic[subscribedIterator] == '#';
}

static bool topicMatcherInternalCheckForEqualCharacterIn(const char *subscribedTopic,
                                                         const char *publishedTopic,
                                                         int subscribedIterator,
                                                         int publishedIterator) {
    return subscribedTopic[subscribedIterator] == publishedTopic[publishedIterator];
}

static bool topicMatcherInternalCheckForSingleLevelWildcardIn(const char *subscribedTopic,
                                                              int subscribedIterator) {
    return subscribedTopic[subscribedIterator] == '+';
}

static bool topicMatcherInternalCheckIfWildcardEndedIn(const char *publishedTopic,
                                                       int publishedIterator) {
    return publishedTopic[publishedIterator] == '/';
}

/* endregion */
