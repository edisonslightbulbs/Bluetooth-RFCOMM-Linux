#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "client.h"
#include "macros.hpp"

#define DEBUG_CLIENT 1
#if DEBUG_CLIENT == 1
#define START_DELAY while (true) {
#define STOP_DELAY                                                             \
    delay(1);                                                                  \
    }
#else
#define START_DELAY
#define STOP_DELAY
#endif

void delay(unsigned int secs)
{
    unsigned int retTime = time(0) + secs;
    while (time(0) < retTime)
        ;
}

void clientCall(int channel, char deviceAddress[18])
{
    struct sockaddr_rc address = { 0 };

    // allocate a socket
    int allocatedSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    address.rc_family = AF_BLUETOOTH;
    address.rc_channel = channel;
    str2ba(deviceAddress, &address.rc_bdaddr);

    // connect to remote device
    int callStatus, writeStatus;
    callStatus
        = connect(allocatedSocket, (struct sockaddr*)&address, sizeof(address));

    if (callStatus == 0) {
        START_DELAY
        writeStatus = write(allocatedSocket, "hello!", 6);
        printf("-- sent: \"hello!\" \n");
        STOP_DELAY
    } else {
        perror("-- failed to connect to remote device");
    }

    close(allocatedSocket);
}
