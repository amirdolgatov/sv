//
// Created by amir on 14.01.23.
//

#ifndef SV_PROJECT_SV_STRUCTURES_H
#define SV_PROJECT_SV_STRUCTURES_H

#endif //SV_PROJECT_SV_STRUCTURES_H

#define TPID_802_1_Q 0x8100

#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <cstring>
#include <byteswap.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <string>
#include <iostream>


struct VLAN_tag{
    uint16_t TPID = 0;
    uint16_t TCI = 0;
};


class EthernetFrame{
public:
    /// Конструктор

    EthernetFrame(ether_header &eth_hdr, uint16_t VLAN_ID, uint16_t VLAN_Priority){

        struct ether_header *eh = (struct ether_header *) this->raw_buffer;
        uint16_t ether_type = bswap_16(eth_hdr.ether_type);

        memcpy(eh->ether_dhost, &(eth_hdr.ether_dhost), 6);
        memcpy(eh->ether_shost, &(eth_hdr.ether_shost), 6);
        memcpy(&eh->ether_type, &ether_type, 2);

        this->frame_payload = this->raw_buffer + sizeof(struct ether_header);     // shift of pointer to payload
        this->frame_size += sizeof(struct ether_header);

        if (VLAN_ID != 0){                                                        // checking tagged or untagged frame
            struct VLAN_tag *tag = (struct VLAN_tag*) &raw_buffer[12];
            tag->TPID = bswap_16(TPID_802_1_Q);                                   // 0x8100
            tag->TCI = bswap_16((VLAN_Priority << 13) | VLAN_ID);                 // Priority + VID
            memcpy(&raw_buffer[12] + sizeof(struct VLAN_tag), &ether_type, 2);

            this->frame_payload += sizeof(struct VLAN_tag);
            this->frame_size += sizeof(struct VLAN_tag);
        }
        for (int i = this->frame_size; i < 500; ++i) {
            raw_buffer[i] = 't';
        }
    }

    EthernetFrame(){}

    /// Поля

    u_char *frame_payload;
    u_char raw_buffer[1500];
    int frame_size = 0;

    //u_char *current_ptr = &raw_buffer[14];

    /// Методы

    u_char *get_raw_buffer(){
        return this->raw_buffer;
    }

    int get_frame_size(){
        return frame_size;
    }

};



