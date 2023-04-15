//
// Created by amir on 14.03.23.
//

#ifndef SV_PROJECT_RAW_SOCKET_H
#define SV_PROJECT_RAW_SOCKET_H

#endif //SV_PROJECT_RAW_SOCKET_H

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

#include "SV_Frame.h"


#define DEFAULT_IF	"lo"
#define BUF_SIZ	1024

class Raw_Socket {
public:

    /// Поля

    int sockfd;
    struct ifreq if_idx;
    struct ifreq if_mac;
    int tx_len = 0;
    char *sendbuf;
    struct sockaddr_ll socket_address;
    char ifName[IFNAMSIZ];


    /// методы

    Raw_Socket(SV_Frame& frame);
    int send_packet();
    long timespec_diff(timespec& t1, timespec& t2);
    long sleep_time_calc(timespec& end, timespec& start);
};