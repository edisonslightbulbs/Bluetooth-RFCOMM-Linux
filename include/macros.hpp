#ifndef MACROS_HPP
#define MACROS_HPP

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <sys/socket.h>

#define UUID uuid_t
#define LIST sdp_list_t
#define DATA sdp_data_t
#define RECORD sdp_record_t
#define MAC_ADDRESS bdaddr_t
#define SESSION sdp_session_t

#endif // MACROS_HPP
