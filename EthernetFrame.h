//
// Created by amir on 14.01.23.
//

#ifndef SV_PROJECT_SV_STRUCTURES_H
#define SV_PROJECT_SV_STRUCTURES_H

#endif //SV_PROJECT_SV_STRUCTURES_H

#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <cstring>
#include <byteswap.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <string>
#include <iostream>


class EthernetFrame{
public:
    /// Конструктор

    EthernetFrame(ether_header &settings){
        memcpy(this->eh->ether_dhost, &(settings.ether_dhost), 6);
        memcpy(this->eh->ether_shost, &(settings.ether_shost), 6);
        memcpy(&this->eh->ether_type, &(settings.ether_type), 2);
    }

    EthernetFrame(){}

public:
    u_char *get_raw_ptr(){
        return &raw_buffer[14];
    }
    /// Поля
    struct ether_header *eh = (struct ether_header *) raw_buffer;

    u_char raw_buffer[1500];

    //u_char *current_ptr = &raw_buffer[14];

    /// Методы




};



