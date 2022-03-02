#include "communicationEndpoint.h"
#include "subscriber.h"
#include "posting.h"

// ONLY FOR TESTING

void publish(Posting posting) {}

void subscribe(char *topic, Subscriber subscriber) {}

void unsubscribe(char *topic, Subscriber subscriber) {}

void subscribeRaw(char *topic, Subscriber subscriber) {}

void unsubscribeRaw(char *topic, Subscriber subscriber) {}

char *ID() {
    return "test";
}
