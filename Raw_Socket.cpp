//
// Created by amir on 14.03.23.
//

#include "raw_socket.h"


Raw_Socket:: Raw_Socket(SV_Frame& frame){

    strcpy(ifName, DEFAULT_IF);

    /* Open RAW socket to send on */
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
        perror("socket");
    }

    /* Get the index of the interface to send on */
    memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
        perror("SIOCGIFINDEX");

    /* Get the MAC address of the interface to send on */
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
        perror("SIOCGIFHWADDR");

    /* Index of the network device */
    socket_address.sll_ifindex = if_idx.ifr_ifindex;


    this->tx_len = frame.get_frame_size();
    this->sendbuf = (char *)frame.get_raw_buffer();

}


int Raw_Socket::send_packet() {

    struct timespec rem, start, end;
    struct timespec sleep_time{
            .tv_sec = 0,
            .tv_nsec = 200000
    };

    long A = 250000, tmp = 0;


    /* Send packet */

    while(1){

        if (sendto(sockfd, sendbuf, this->tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
            printf("Send failed\n");
        nanosleep(&sleep_time, &rem);
        }
}
