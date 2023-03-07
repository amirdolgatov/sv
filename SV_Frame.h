//
// Created by amir on 06.03.23.
//

#ifndef SV_PROJECT_SV_FRAME_H
#define SV_PROJECT_SV_FRAME_H

#endif //SV_PROJECT_SV_FRAME_H

#include "EthernetFrame.h"
#include "SV_attributes.h"

///  атрибуты пакета которым необходимо задавать значение
struct SV_attributes{
    String_attribute svID;
    String_attribute datSet;
    Type_attribute<uint16_t> smpCnt;
    Type_attribute<uint32_t> confrev;
    Type_attribute<uint64_t> refrTm;
    Type_attribute<uint8_t> smpSynch;
    Type_attribute<uint16_t> smpRate;
};

struct SV_header{
    uint16_t APPID = 0x4001;
    uint16_t lengh = 0;
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
};

struct General_SV_Settings{
    ether_header eth_hdr;
    uint16_t VLAN_Priority;
    uint16_t VLAN_ID;
    SV_header sv_hdr;
    SV_attributes savPdu_data;
};

class SV_Frame: public EthernetFrame{
public:


    struct SV_header{
        uint16_t APPID = 0x4001;
        uint16_t lengh = 0;
        uint16_t reserved1 = 0;
        uint16_t reserved2 = 0;
    } sv_hdr;



    SV_attributes asdu_attr;



    /// методы

    SV_Frame(){}
    SV_Frame(General_SV_Settings sv_settings): EthernetFrame(sv_settings.eth_hdr, sv_settings.VLAN_Priority, sv_settings.VLAN_ID){}

    void build_SV_data(General_SV_Settings& settings);
    Container build_savPdu(void);
};
