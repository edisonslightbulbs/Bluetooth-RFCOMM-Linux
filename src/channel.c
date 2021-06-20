// #include <stdio.h>
#include <stdlib.h>
// #include <string.h>

// #include <bluetooth/bluetooth.h>
// #include <bluetooth/sdp.h>
// #include <bluetooth/sdp_lib.h>

#include "macros.hpp"
#include "channel.h"

int getChannel(uint8_t *uuid, char deviceAddress[18])
{
    /*  connect to sdp server: device must be ON but server need not be running
     */
    uint8_t address[6];
    str2ba(deviceAddress, (MAC_ADDRESS*)&address);
    SESSION* session
    = sdp_connect(BDADDR_ANY, (MAC_ADDRESS*)&address, SDP_RETRY_IF_BUSY);
    if (!session) {
        fprintf(stderr, "-- can't connect to sdp server! \n");
        exit(-1);
    }

    /* create query lists */
    UUID deviceUuid;
    int range = 0x0000ffff; // from 0000 to ffff
    sdp_uuid128_create(&deviceUuid, uuid);
    LIST* responseList;
    LIST* searchList = sdp_list_append(NULL, &deviceUuid);
    LIST* attrIdList = sdp_list_append(NULL, &range);

    /* search for records: server must be running */
    int success = sdp_service_search_attr_req(
            session, searchList, SDP_ATTR_REQ_RANGE, attrIdList, &responseList);
    if (success) {
        fprintf(stderr, "-- search failed! \n");
        exit(-1);
    }

    /* check responses */
    success = sdp_list_len(responseList);
    if (success <= 0) {
        fprintf(stderr, "-- no responses! \n");
        exit(-1);
    }

    /* process responses */
    int channel = 0;
    LIST* responses = responseList;
    while (responses) {
        LIST *protoList, *protocol;
        RECORD* record = (RECORD*)responses->data;
        success = sdp_get_access_protos(record, &protoList);
        if (success) {
            fprintf(stderr, "-- can't access protocols! \n");
            exit(-1);
        }

        protocol = protoList;
        while (protocol) {
            LIST* pds;
            int protocolCount = 0;
            pds = (LIST*)protocol->data;

            while (pds) { // loop thru all pds
                DATA* d;
                int dtd;
                d = pds->data;
                while (d) {
                    dtd = d->dtd;
                    switch (dtd) {
                        case SDP_UUID16:
                        case SDP_UUID32:
                        case SDP_UUID128:
                            protocolCount = sdp_uuid_to_proto(&d->val.uuid);
                            break;
                        case SDP_UINT8:
                            if (protocolCount == RFCOMM_UUID) {
                                channel = d->val.uint8; // save channel id
                            }
                            break;
                        default:
                            break;
                    }
                    d = d->next; // to next data unit
                }
                pds = pds->next; // to next pds
            }
            sdp_list_free((LIST*)protocol->data, 0);

            protocol = protocol->next; // to next protocol
        }
        sdp_list_free(protoList, 0);

        responses = responses->next; // to next response
    }

    // assert channel # no less that 1:
    // [ in principle, must be in range 1 - 30 ]
    if (channel < 1) {
        fprintf(stderr, "-- no rfcomm channel found using UUID: ");
        fprintf(stderr, "d8308c4e-9469-4051-8adc-7a2663e415e2 \n");
        exit(0);
    }

    printf("-- rfcomm channel: %d\n", channel);
    return (channel);
}