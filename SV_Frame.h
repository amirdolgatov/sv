//
// Created by amir on 06.03.23.
//

#ifndef SV_PROJECT_SV_FRAME_H
#define SV_PROJECT_SV_FRAME_H

#endif //SV_PROJECT_SV_FRAME_H

#include "EthernetFrame.h"
#include "SV_attributes.h"

/// настройки для потока
struct ASDU_settings{
    std::string svID = "";   // 7 *
    std::string datSet = ""; // 7
    uint16_t smpCnt = 786;          // 2
    uint32_t confrev = 665;         // 4
    uint64_t refrTm = 999;          // 8
    uint8_t smpSynch = 111;         // 1
    uint16_t smpRate = 1;           // 2
    uint32_t noASDU = 4;            // 4
};

///  атрибуты пакета которым необходимо задавать значение
struct savPdu_entries{
    Container savPdu = Container(0x60);
    Container seqASDU = Container(0xA2);
    Container ASDU = Container(0x30);
    Type_attribute<uint32_t> noASDU;
    String_attribute svID;
    String_attribute datSet;
    Type_attribute<uint16_t> smpCnt;
    Type_attribute<uint32_t> confrev;
    Type_attribute<uint64_t> refrTm;
    Type_attribute<uint8_t> smpSynch;
    Type_attribute<uint16_t> smpRate;
    Seq_of_Data seq_of_Data;
};

struct SV_HDR{
    uint16_t APPID = 0x4001;
    uint16_t lengh = 0;
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
};
/// All parameters of Sampled Values data bloks

struct General_SV_Settings{
    ether_header eth_hdr;             /// Destination MAC
    uint16_t VLAN_Priority;           /// VLAN parameters
    uint16_t VLAN_ID;
    SV_HDR sv_header;
    ASDU_settings asdu_parametrs;           /// SV ID, datSet, confRev

};

class SV_Frame: public EthernetFrame{
public:
    /// поля
    savPdu_entries attributes;

    /// методы

    SV_Frame(){}
    SV_Frame(General_SV_Settings sv_settings);

    void build_SV_frame(General_SV_Settings& settings);
    void build_savPdu();
    u_char* set_ASDU_to_frame(Attribute *attribute_ptr, u_char* frame_ptr);
    void print_frame();


    void traverse(Attribute* atr_ptr);
    void build (Attribute* atr_ptr);
};
