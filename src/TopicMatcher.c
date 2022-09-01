#include "TopicMatcher.h"
#include "TopicMatcherInternal.h"
#include <stdbool.h>
#include <string.h>

/* region PUBLIC HEADER FUNCTIONS */

bool TopicMatcherCheckIfTopicMatches(char *subscribedTopic, char *publishedTopic) {
    if (TopicMatcherInternalTopicsAreEqual(subscribedTopic, publishedTopic)) {
        return true;
    }
    int subscribedIterator = 0;
    int publishedIterator = 0;

    while (publishedIterator < strlen(publishedTopic)) {
        if (TopicMatcherInternalMultiLevelWildcardIn(subscribedTopic, subscribedIterator)) {
            return true;
        }
        if (TopicMatcherInternalCheckForEqualCharacterIn(subscribedTopic, publishedTopic,
                                                         subscribedIterator, publishedIterator)) {
            subscribedIterator++;
            publishedIterator++;
        } else if (TopicMatcherInternalCheckForSingleLevelWildcardIn(subscribedTopic,
                                                                     subscribedIterator)) {
            publishedIterator++;
            if (TopicMatcherInternalCheckIfWildcardEndedIn(publishedTopic, publishedIterator)) {
                subscribedIterator++;
            }
        } else {
            return false;
        }
    }
    if (TopicMatcherInternalSubscribedTopicHasCharactersLeft(subscribedTopic, subscribedIterator)) {
        return false;
    }
    return true;
}

/* endregion */

/* region INTERNAL HEADER FUNCTIONS */

static bool TopicMatcherInternalTopicsAreEqual(char *subscribedTopic, char *publishedTopic) {
    return strcmp(subscribedTopic, publishedTopic) == 0;
}

static bool TopicMatcherInternalSubscribedTopicHasCharactersLeft(char *subscribedTopic,
                                                                 int subscribedIterator) {
    return subscribedIterator < strlen(subscribedTopic) &&
           subscribedTopic[subscribedIterator] != '+';
}

static bool TopicMatcherInternalMultiLevelWildcardIn(const char *subscribedTopic,
                                                     int subscribedIterator) {
    return subscribedTopic[subscribedIterator] == '#';
}

static bool TopicMatcherInternalCheckForEqualCharacterIn(const char *subscribedTopic,
                                                         const char *publishedTopic,
                                                         int subscribedIterator,
                                                         int publishedIterator) {
    return subscribedTopic[subscribedIterator] == publishedTopic[publishedIterator];
}

static bool TopicMatcherInternalCheckForSingleLevelWildcardIn(const char *subscribedTopic,
                                                              int subscribedIterator) {
    return subscribedTopic[subscribedIterator] == '+';
}

static bool TopicMatcherInternalCheckIfWildcardEndedIn(const char *publishedTopic,
                                                       int publishedIterator) {
    return publishedTopic[publishedIterator] == '/';
}

/* endregion */