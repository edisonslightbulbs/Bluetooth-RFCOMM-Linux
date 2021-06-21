#ifndef CLIENT_H
#define CLIENT_H

/**
 * clientCall
 *   Calls a remote server over a RFCOMM (TX) channel.
 *
 *   @ref
 *     https://people.csail.mit.edu/albert/bluez-intro/x502.html
 *
 *   accessed
 *     05:22 18 June 2021
 */
void clientCall(int channel, char deviceAddress[18]);

#endif
