#include "exp2imas_mds.h"

#include <stdlib.h>
#include <mdslib.h>
#include <logging/logging.h>
#include <clientserver/stringUtils.h>

#define status_ok(status) (((status) & 1) == 1)

static int get_signal_length(const char* signal)
{
    /* local vars */
    int dtype_long = DTYPE_LONG;
    char buf[2048];
    int size;
    int null = 0;
    int idesc = descr(&dtype_long, &size, &null);
    int status;

    /* put SIZE() TDI function around signal name */
    sprintf(buf, "%s; SIZE(_sig);", signal);

    /* use MdsValue to get the signal length */
    status = MdsValue(buf, &idesc, &null, NULL);
    if (((status & 1) != 1)) {
        IDAM_LOGF(UDA_LOG_ERROR, "Unable to get length of %s.\n", signal);
        return -1;
    }

    /* return signal length */
    return size;

}

int mds_get(const char* signalName, int shot, float** time, float** data, int* len, int time_dim)
{
    int status;

    /* Connect to MDSplus */
    int socket = MdsConnect("mdsplus.jet.efda.org:8000");
    if (socket == -1) {
        IDAM_LOG(UDA_LOG_ERROR, "Error connecting to mdsplus.jet.efda.org.\n");
        return -1;
    }

    bool is_tdi = false;
    if (STR_STARTSWITH(signalName, "%TDI%")) {
        signalName = &signalName[5];
        is_tdi = true;
    }

    char signal[2048];

    char* shot_pos = strstr(signalName, "%SHOT%");
    if (shot_pos != NULL) {
        size_t offset = shot_pos - signalName;
        strncpy(signal, signalName, offset);
        size_t l = sprintf(signal + offset, "%d", shot);
        strcpy(signal + offset + l, shot_pos + 6);
    } else if (is_tdi) {
        sprintf(signal, "%s", signalName);
    } else {
        sprintf(signal, "_sig=jet(\"%s\",%d);", signalName, shot);
    }

    *len = get_signal_length(signal);

    if (*len < 0) {
        IDAM_LOG(UDA_LOG_ERROR, "Unable to get signal length.\n");
        return -1;
    }

    *time = malloc(*len * sizeof(float));
    *data = malloc(*len * sizeof(float));

    int null = 0;
    int dtype_float = DTYPE_FLOAT;

    int fdesc = descr(&dtype_float, *time, len, &null);
    int rlen = 0;

    char buf[2048];

    //Get time data
    sprintf(buf, "%s; dim_of(_sig, %d);", signal, time_dim - 1);

    status = MdsValue(buf, &fdesc, &null, &rlen, NULL);
    if (((status & 1) != 1)) {
        IDAM_LOG(UDA_LOG_ERROR, "Unable to get signal.\n");
        return -1;
    }

    fdesc = descr(&dtype_float, *data, len, &null);

    //Get data
    sprintf(buf, "%s; _sig;", signal);

    status = MdsValue(buf, &fdesc, &null, &rlen, NULL);
    if (((status & 1) != 1)) {
        IDAM_LOG(UDA_LOG_ERROR, "Unable to get signal.\n");
        return -1;
    }

    MdsDisconnect();

    return 0;
}