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

//    /* Address length*/
//    socket_address.sll_halen = ETH_ALEN;
//
//    /* Destination MAC */
//    socket_address.sll_addr[0] = frame.get_raw_buffer()[0];
//    socket_address.sll_addr[1] = frame.get_raw_buffer()[1];
//    socket_address.sll_addr[2] = frame.get_raw_buffer()[2];
//    socket_address.sll_addr[3] = frame.get_raw_buffer()[3];
//    socket_address.sll_addr[4] = frame.get_raw_buffer()[4];
//    socket_address.sll_addr[5] = frame.get_raw_buffer()[5];

    this->tx_len = frame.get_frame_size();
    this->sendbuf = (char *)frame.get_raw_buffer();


}

long Raw_Socket::timespec_diff(timespec& t1, timespec& t2){
    if (t2.tv_nsec < t1.tv_nsec)
        return 1000000000 + t2.tv_nsec - t1.tv_nsec;
    return t2.tv_nsec - t1.tv_nsec;
}

long Raw_Socket::sleep_time_calc(timespec& end, timespec& start){
    long tmp = timespec_diff(end, start);
    if (tmp < 250000)
        return 250000;
    return 500000 - tmp;
}



int Raw_Socket::send_packet() {

    struct timespec rem, start, end;
    struct timespec sleep_time{
            .tv_sec = 0,
            .tv_nsec = 200000
    };

    long A = 250000, tmp = 0;

//    clock_gettime(CLOCK_REALTIME, &end);
//    nanosleep(&sleep_time, &rem);

    /* Send packet */

    while(1){
        //clock_gettime(CLOCK_REALTIME, &start);
        //sleep_time.tv_nsec = sleep_time_calc(end, start);
        //tmp = timespec_diff(end, start);
        //A = (A)/2 + tmp/2;
        if (sendto(sockfd, sendbuf, this->tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
            printf("Send failed\n");
//        sleep_time.tv_nsec = 500000 - A;
//        clock_gettime(CLOCK_REALTIME, &end);
        nanosleep(&sleep_time, &rem);
        }
}
