#ifndef CLIENT_H
#define CLIENT_H

/**
 * client.h
 *   Sets up an RFCOMM client for communication over a Bluetooth channel.
 *
 *   @ref
 *     https://people.csail.mit.edu/albert/bluez-intro/x502.html
 *
 *   accessed
 *     18 June 2021
 */
void client(int channel, char deviceAddress[18]);

#endif