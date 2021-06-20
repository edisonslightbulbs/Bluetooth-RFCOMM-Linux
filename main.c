// RFCOMM connection between Linux system and Android device.
//
#include <stdint-gcc.h>
#include <stdlib.h>

#include "channel.h"
#include "client.h"

// todo: remove post initial implementation
#include "macros.hpp"

void setupClient(char deviceAddress[18])
{

    // determine RFCOMM client-server channel for
    // UUID: d8308c4e-9469-4051-8adc-7a2663e415e2
    static uint8_t CLIENT_CHANNEL_UUID[16] = { 0xd8, 0x30, 0x8c, 0x4e, 0x94,
        0x69, 0x40, 0x51, 0x8a, 0xdc, 0x7a, 0x26, 0x63, 0xe4, 0x15, 0xe2 };
    int clientChannel = getChannel(CLIENT_CHANNEL_UUID, deviceAddress);

    // setup client
    client(clientChannel, deviceAddress);
}

void setupServer(char deviceAddress[18]) { }

#define DEVICE_ADDRESS "C0:8C:71:61:34:8C" // android device

int main()
{
    char deviceAddress[18] = DEVICE_ADDRESS;

    // setupClient(DEVICE_ADDRESS);
    // setupServer(DEVICE_ADDRESS);

    struct sockaddr_rc localAddress = { 0 }, remoteAddress = { 0 };
    char buf[1024] = { 0 };
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
    localAddress.rc_channel = 20;

    // bind communication socket
    status = bind(
        newSocket, (struct sockaddr*)&localAddress, sizeof(localAddress));
    if (status < 0) {
        fprintf(stderr, "-- socket binding failed!\n");
        exit(-1);
    }

    // listen for new connection
    status = listen(newSocket, 1);
    if (status < 0) {
        fprintf(stderr, "-- failed to listen for new connections!\n");
        exit(-1);
    }

    // wait for client to connect
    printf("-- waiting for client to connect...\n");
    int client = accept(newSocket, (struct sockaddr*)&remoteAddress, &opt);

    if (client < 0) {
        fprintf(stderr, "-- failed to connect to client\n");
        exit(-1);
    } else {
        printf("-- client connected successfully\n");
    }

    // --- ba2str(&remoteAddress.rc_bdaddr, buf);
    // --- printf("accepted connection from %s\n", buf);
    // --- memset(buf, 0, sizeof(buf));

    // --- // read data from the client
    // --- while (true) {
    // ---     int bytesRead = read(client, buf, sizeof(buf));
    // ---     if (bytesRead > 0) {
    // ---         printf("received [%s]\n", buf);
    // ---     }
    // ---     delay(1);
    // --- }

    // --- // close connection
    // --- close(client);
    // --- close(allocatedSocket);
    return 0;
}
