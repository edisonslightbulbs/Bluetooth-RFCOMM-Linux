#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "macros.hpp"
#include "client.h"

void delay(unsigned int secs)
{
    unsigned int retTime = time(0) + secs;
    while (time(0) < retTime);
}

void client(int channel, char deviceAddress[18])
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
    callStatus = connect(allocatedSocket, (struct sockaddr*)&address, sizeof(address));

        if (callStatus == 0) {
            while (true) {
                writeStatus = write(allocatedSocket, "hello!", 6);
                printf("-- sent: \"hello!\" \n");
                delay(1);
            }
        } else { perror("-- failed to connect to remote device"); }

    close(allocatedSocket);
}