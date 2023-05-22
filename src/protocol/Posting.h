#ifndef ELASTIC_AI_RUNTIME_C_POSTING_H
#define ELASTIC_AI_RUNTIME_C_POSTING_H

#include <stdbool.h>

/*!
 * A struct representing a posting which can be published
 */
struct posting {
    /*! The topic to where the posting is published to  */
    char *topic;
    /*! The payload of the posting */
    char *data;
    /*! retain message inside topic on MQTT server */
    bool retain;
};
typedef struct posting posting_t;

#endif /* ELASTIC_AI_RUNTIME_C_POSTING_H */
