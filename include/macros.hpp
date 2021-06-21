// macros to support learnability
//
#ifndef MACROS_HPP
#define MACROS_HPP

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <time.h>

#define UUID uuid_t
#define ADDRESS bdaddr_t

#define LIST sdp_list_t
#define DATA sdp_data_t
#define RECORD sdp_record_t
#define SESSION sdp_session_t

#define RX_TX_DELAY 0

#if RX_TX_DELAY == 1
#define SECONDS 1
#define BEGIN_DELAY_ENVIRONMENT while (true) {
#define END_DELAY_ENVIRONMENT                                                  \
    unsigned int retTime = time(0) + SECONDS;                                  \
    while (time(0) < retTime)                                                  \
        ;                                                                      \
    }
#else
#define BEGIN_DELAY_ENVIRONMENT
#define END_DELAY_ENVIRONMENT
#endif

#endif // MACROS_HPP
