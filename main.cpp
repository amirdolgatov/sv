#include <iostream>
#include "raw_socket.h"
#include <typeinfo>

#include <thread>

#define TIME 5000000

int main() {


    struct ether_header mac {
            .ether_dhost = {0x01, 0x0C, 0xCD, 0x04, 0x00, 0x01},
            .ether_shost = {0xE8, 0x6C, 0xC7, 0x73, 0xA7, 0x1D},
            .ether_type = 0x88ba
    };

    ASDU_settings asdu{
        .svID = "ABCl;;lk;gk;gk;dlgk;lskgl;dfDE",   // 7 *
        .datSet = "DATAgdfglgfd;slklgksdg", // 7
        .smpCnt = 0,          // 2
        .confrev = 1,         // 4
        .refrTm = 0,          // 8
        .smpSynch = 2,         // 1
        .smpRate = 1,           // 2
        .noASDU = 8            // 4
    };

    struct General_SV_Settings settings {
        .eth_hdr = mac,             /// Destination MAC
        .VLAN_Priority = 10,           /// VLAN parameters
        .VLAN_ID = 2,
        .asdu_parametrs = asdu           /// SV ID, datSet, confRev
    };

    SV_Frame frame = SV_Frame(settings);
    //frame.print_frame();
    Raw_Socket socket = Raw_Socket(frame);

    for(auto k : frame.values_ptr)
        std::cout << "shift from begin " << k - frame.raw_buffer << std::endl;

//    std::thread thr1( &Raw_Socket:: send_packet, socket);
//
//    thr1.join();

socket.send_packet();

    return 0;
}


