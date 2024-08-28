#ifndef ENV5_STATUS_H
#define ENV5_STATUS_H

#include <stdlib.h>

#define STATUS_STATE_ONLINE "ONLINE"
#define STATUS_STATE_OFFLINE "OFFLINE"

#define STATUS_ID "ID"
#define STATUS_TYPE "TYPE"
#define STATUS_STATE "STATE"

#define STATUS_DATA "DATA"
#define STATUS_FPGA "FPGA"
#define STATUS_VERSION "VERSION"

#define STATUS_APPLICATIONS "STORED_APPS"

struct status {
    char *id;
    char *type;
    char *state;

    char *data;
    char *fpga;
    char *version;

    char *storedApplications;
};

typedef struct status status_t;

#endif // ENV5_STATUS_H
