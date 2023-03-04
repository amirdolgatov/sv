//
// Created by amir on 30.12.22.
//

#ifndef SV_PROJECT_SV_CLASSES_H
#define SV_PROJECT_SV_CLASSES_H

#endif //SV_PROJECT_SV_CLASSES_H

#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <netinet/if_ether.h>
#include <map>
#include "SV_attributes.h"
#include "EthernetFrame.h"





/// настройки для потока
struct SV_attribute_values{
    std::string svID = "ABCDEFGrtdrtertdfgdfgdfgdfgdf";
    std::string datSet = "DATASET_cvxcbcbcvbgbcfgfghfgh";
    uint16_t smpCnt = 786;
    uint32_t confrev = 665;
    uint64_t refrTm = 999;
    uint8_t smpSynch = 111;
    uint16_t smpRate = 1;
    uint32_t noASDU = 1;
    instant_values val;
};

///  атрибуты пакета которым необходимо задавать значение
struct SV_attributes{
    Simple_attribute svID;
    Simple_attribute datSet;
    TSimple_attribute<uint16_t> smpCnt;
    TSimple_attribute<uint32_t> confrev;
    TSimple_attribute<uint64_t> refrTm;
    TSimple_attribute<uint8_t> smpSynch;
    TSimple_attribute<uint16_t> smpRate;
    Simple_attribute_seq seq_of_Data;
};

struct SV_header{
    uint16_t APPID = 0x4001;
    uint16_t lengh = 0;
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
};

class SV_generator {
public:

    const std::map<int, std::string> sv_tags = {
            {0x60, "savPdu"},
            {0xA2, "seqASDU"},
            {0x30, "ASDU"},
            {0x80, "svID"},
            {0x81, "datSet"},
            {0x82, "smpCnt"},
            {0x83, "confRev"},
            {0x84, "refrTm"},
            {0x85, "smpSynch"},
            {0x86, "smpRate"},
            {0x87, "seqData"}
    };

    struct ether_header settings {
        .ether_dhost = {0x01, 0x0C, 0xCD, 0x04, 0x00, 0x01},
        .ether_shost = {0xE8, 0x6C, 0xC7, 0x73, 0xA7, 0x1D},
        .ether_type = 0xba88
    };

    //SV_header;
    SV_attributes asdu_attr;

    std::list<Attribute*> list_of_attribute;

    EthernetFrame frame = EthernetFrame(settings);

    SV_header hdr;
    Container savPdu = Container(0x60);
    Container seqASDU = Container(0xA2);
    Container ASDU = Container(0x30);

    TSimple_attribute<uint32_t> noASDU;

    SV_attributes initialize_ASDU(SV_attribute_values& source);

    Container build_SV_block();

    /// методы

    void build_frame();
    u_char *traverse(Attribute *attr, u_char* frame_ptr);

    SV_generator(){}

    void print_frame();
};

