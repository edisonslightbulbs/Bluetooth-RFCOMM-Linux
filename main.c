// RFCOMM connection between Linux system and Android device.
//
#include "channel.h"
#include "client.h"
#include "server.h"

int main()
{
    // determine RFCOMM client-server channel for
    // UUID: d8308c4e-9469-4051-8adc-7a2663e415e2
    //  n.b. make sure UUID on the android device
    //  is the same as this one.
    static uint8_t CHANNEL_UUID[16] = { 0xd8, 0x30, 0x8c, 0x4e, 0x94, 0x69,
        0x40, 0x51, 0x8a, 0xdc, 0x7a, 0x26, 0x63, 0xe4, 0x15, 0xe2 };

    char deviceAddress[18] = "C0:8C:71:61:34:8C"; // android address
    int clientChannel = getChannel(CHANNEL_UUID, deviceAddress);

    // make a client call to a server device
    clientCall(clientChannel, deviceAddress);

    // create a local server and open a listening channel
    setupSever();

    return 0;
}
