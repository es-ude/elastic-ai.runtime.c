#ifndef ENV5_STATUS_H
#define ENV5_STATUS_H

#include <stdlib.h>

#define STATUS_STATE_ONLINE "ONLINE"
#define STATUS_STATE_OFFLINE "OFFLINE"

#define STATUS_TYPE_DEVICE "DEVICE"
#define STATUS_TYPE_TWIN "TWIN"

#define STATUS_ID "ID"
#define STATUS_STATE "STATE"
#define STATUS_MEASUREMENTS "MEASUREMENTS"
#define STATUS_TYPE "TYPE"

struct status {
    char *id;
    char *type;
    char *measurements;
    char *state;
};

typedef struct status status_t;

#endif // ENV5_STATUS_H
