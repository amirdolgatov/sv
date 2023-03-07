//
// Created by amir on 06.03.23.
//

#include "SV_Frame.h"
#include "SV_attributes.h"


void SV_Frame::build_SV_data(struct General_SV_Settings& sv_settings) {
    struct SV_header *pointer = (struct SV_header *) &raw_buffer[14];
    pointer->APPID = sv_settings.sv_hdr.APPID;
    pointer->lengh = sv_settings.sv_hdr.lengh;
    pointer->reserved1 = sv_settings.sv_hdr.reserved1;
    pointer->reserved2 = sv_settings.sv_hdr.reserved2;
    /// продолжить отсюда

}

Container SV_Frame::build_savPdu() {
    Container savPdu = Container(0x60);
    Container seqASDU = Container(0xA2);
    Container ASDU = Container(0x30);

    Type_attribute<uint32_t> noASDU;
}