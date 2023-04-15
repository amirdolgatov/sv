//
// Created by amir on 12.04.23.
//
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <time.h>

#ifndef SV_PROJECT_SENDER_H
#define SV_PROJECT_SENDER_H

#endif //SV_PROJECT_SENDER_H


class Sender{
public:
    u_char *sendbuf;

    timespec delay = {
            .tv_sec = 0,
            .tv_nsec = 250000
    };

    void set_delay(long _delay){
        this->delay.tv_nsec = _delay;
    }

    void set_buf_pointer(u_char *new_ptr){
        this->sendbuf = new_ptr;
    }
};
