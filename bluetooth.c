/**
 * bluetooth.c
 *   Determines a service channel
 */

#include "bluetooth/bluetooth.h"
#include "bluetooth/sdp.h"
#include "bluetooth/sdp_lib.h"
#include "stdio.h"
#include "string.h"

//#include "stdlib.h"
//#include "ctype.h"
//#include "unistd.h"

#define BYTE uint8_t
#define UUID uuid_t
#define LIST sdp_list_t
#define SESSION sdp_session_t
#define RECORD sdp_record_t
#define DATA sdp_data_t

/* service device | android device must be in server mode! */
#define MAC_ADDRESS "C0:8C:71:61:34:8C"

int getServiceChannel(BYTE* uuid)
{
    UUID svc_uuid;

    int range;
    int n;

    BYTE address[6];
    int channel = 0;

    /*  CONNECT TO SDP SERVER
     *  Note: device must be ON but server need not be running  */

    str2ba(MAC_ADDRESS, (bdaddr_t*)&address);
    SESSION* session
        = sdp_connect(BDADDR_ANY, (bdaddr_t*)&address, SDP_RETRY_IF_BUSY);
    if (!session) {
        fprintf(stderr, "can't connect to sdp server\n");
        return (0);
    }

    // create query lists
    range = 0x0000ffff; // from 0000 to ffff
    sdp_uuid128_create(&svc_uuid, uuid);

    LIST* responseList;
    LIST* searchList = sdp_list_append(NULL, &svc_uuid);
    LIST* attrIdList = sdp_list_append(NULL, &range);

    // SEARCH FOR RECORDS
    // (Note: Server must be running)
    n = sdp_service_search_attr_req(
        session, searchList, SDP_ATTR_REQ_RANGE, attrIdList, &responseList);
    if (n) {
        fprintf(stderr, "search failed.\n");
        return (0);
    }

    // CHECK IF ANY RESPONSES
    n = sdp_list_len(responseList);
    if (n <= 0) {
        fprintf(stderr, "no responses.\n");
        return (0);
    }

    // PROCESS RESPONSES
    LIST* responses = responseList;
    while (responses) { // loop thru all responses
        LIST *protoList, *protocol;
        RECORD* record = (RECORD*)responses->data;
        n = sdp_get_access_protos(record, &protoList);
        if (n) {
            fprintf(stderr, "can't get access protocols.\n");
            return (0);
        }

        protocol = protoList;
        while (protocol) {
            LIST* pds;
            int protocolCount = 0;
            pds = (LIST*)protocol->data;

            while (pds) { // loop thru all pds
                DATA* d;
                int dtd;
                d = pds->data;     // get data ptr of pds
                while (d) {        // loop over all data
                    dtd = d->dtd;  // get dtd of data
                    switch (dtd) { // which dtd?
                    case SDP_UUID16:
                    case SDP_UUID32:
                    case SDP_UUID128:
                        protocolCount
                            = sdp_uuid_to_proto(&d->val.uuid); // get proto #
                        break;
                    case SDP_UINT8:
                        if (protocolCount == RFCOMM_UUID) { // proto is rfcomm?
                            channel = d->val.uint8;         // save channel id
                        }
                        break;
                    default:
                        break;
                    }
                    d = d->next; // advance to next data unit
                }
                pds = pds->next; // advance to next pds
            }
            sdp_list_free((LIST*)protocol->data, 0);

            protocol = protocol->next; // advance to next protocol
        }
        sdp_list_free(protoList, 0);

        responses = responses->next; // advance to next response
    }

    return (channel); // in range [1-30] or 0 if not found
}

int main()
{
    // server UUID: d8308c4e-9469-4051-8adc-7a2663e415e2
    static BYTE DEVICE_UUID[16] = { 0xd8, 0x30, 0x8c, 0x4e, 0x94, 0x69, 0x40,
        0x51, 0x8a, 0xdc, 0x7a, 0x26, 0x63, 0xe4, 0x15, 0xe2 };

    int channel = getServiceChannel(DEVICE_UUID);

    if (channel > 0) {
        printf("specified service is on channel %d\n", channel);
    } else {
        fprintf(stderr, "  can't find channel\n");
    }

    return 0;
}
