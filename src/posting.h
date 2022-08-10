#ifndef ELASTIC_AI_RUNTIME_C_POSTING_H
#define ELASTIC_AI_RUNTIME_C_POSTING_H

/*!
 * A struct representing a Posting which can be published
 */
typedef struct Posting_t {
    /*! The topic to where the Posting is published to  */
    char *topic;
    /*! The payload of the Posting */
    char *data;
} Posting;

#endif // ELASTIC_AI_RUNTIME_C_POSTING_H
