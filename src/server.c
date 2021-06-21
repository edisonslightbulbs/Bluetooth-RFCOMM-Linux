#include <stdlib.h>
#include <unistd.h>

#include "macros.hpp"
#include "server.h"

void setupSever(int channelID, int connectionCount)
{
    struct sockaddr_rc localAddress = { 0 }, remoteAddress = { 0 };
    socklen_t opt = sizeof(remoteAddress);

    int status;

    // allocate socket
    int newSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (newSocket < 0) {
        fprintf(stderr, "-- rfcomm socket allocation failed!\n");
        exit(-1);
    }

    localAddress.rc_family = AF_BLUETOOTH;
    localAddress.rc_bdaddr = *BDADDR_ANY;
    localAddress.rc_channel = channelID;

    // bind communication socket
    status = bind(
        newSocket, (struct sockaddr*)&localAddress, sizeof(localAddress));
    if (status < 0) {
        fprintf(stderr, "-- socket binding failed!\n");
        exit(-1);
    }

    // listen for new connection
    status = listen(newSocket, connectionCount);
    if (status < 0) {
        fprintf(stderr, "-- failed to listen for new connections!\n");
        exit(-1);
    }

    // wait for client to connect
    printf("-- waiting for client to connect\n");
    int client = accept(newSocket, (struct sockaddr*)&remoteAddress, &opt);

    if (client < 0) {
        fprintf(stderr, "-- failed to connect to client\n");
        exit(-1);
    } else {
        printf("-- client connected successfully\n");
    }

    char buf[1024] = { 0 };
    ba2str(&remoteAddress.rc_bdaddr, buf);
    printf("accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    BEGIN_DELAY_ENVIRONMENT
    long bytesRead = read(client, buf, sizeof(buf));
    if (bytesRead > 0) {
        printf("received [%s]\n", buf);
    }
    END_DELAY_ENVIRONMENT

    // close connection
    close(client);
    close(newSocket);
}
