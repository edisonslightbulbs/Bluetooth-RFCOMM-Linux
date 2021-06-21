#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdint-gcc.h>

/**
 * channel.h
 *   Establishes RFCOMM (TX) channel associated with
 *   a given UUID and MAC address.
 *
 *   @ref
 *     https://people.csail.mit.edu/albert/bluez-intro/x604.html
 *
 *   accessed
 *     18 June 2021
 */
int getChannel(uint8_t* uuid, char deviceAddress[18]);

#endif // CHANNEL_H
