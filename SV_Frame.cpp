//
// Created by amir on 06.03.23.
//

#include "SV_Frame.h"


void SV_Frame::build_SV_frame(struct General_SV_Settings& sv_settings) {
    /// сформируем блок savPdu
    /// set  fields APPID, length, reserved1, reserved2

    struct SV_HDR *pointer = (struct SV_HDR *) this->frame_payload;

    pointer->APPID = bswap_16(sv_settings.sv_header.APPID);
    pointer->lengh = bswap_16( this->attributes.savPdu.length + this->attributes.savPdu.tagSize + this->attributes.savPdu.lengthSize + 8);
    pointer->reserved1 = bswap_16(sv_settings.sv_header.reserved1);
    pointer->reserved2 = bswap_16(sv_settings.sv_header.reserved2);
    this->frame_size += sizeof(struct SV_HDR);
    //traverse(&savPdu);
    set_ASDU_to_frame(&this->attributes.savPdu,  (u_char*)pointer + sizeof(struct SV_HDR));

    this->frame_size += (this->attributes.savPdu.length + attributes.savPdu.tagSize + attributes.savPdu.lengthSize);

}

void SV_Frame::build_savPdu() {

    /// начало формирования структуры (дерева) savPdu
    ///fill of ASDU
    this->attributes.ASDU.add(this->attributes.svID);
    this->attributes.ASDU.add(this->attributes.datSet);
    this->attributes.ASDU.add(this->attributes.smpCnt);
    this->attributes.ASDU.add(this->attributes.confrev);
    this->attributes.ASDU.add(this->attributes.refrTm);
    this->attributes.ASDU.add(this->attributes.smpSynch);
    this->attributes.ASDU.add(this->attributes.smpRate);
    this->attributes.ASDU.add(this->attributes.seq_of_Data);
    ///fill of seqASDU
    for (int i = 0; i < this->attributes.noASDU.value; i++) {
        this->attributes.seqASDU.add(this->attributes.ASDU);
    }
    ///fill of savPdu
    this->attributes.savPdu.add(this->attributes.noASDU);
    this->attributes.savPdu.add(this->attributes.seqASDU);

}

/// перенос значений атрибутов в массив u_char, для последующей отправки
u_char* SV_Frame::set_ASDU_to_frame(Attribute *attribute_ptr, u_char* raw_buffer_ptr) {

    raw_buffer_ptr += attribute_ptr->record_TLV(raw_buffer_ptr);

    if (attribute_ptr->tag[0] & 0x20) {
        for (auto it: attribute_ptr->list_of_children) {
            raw_buffer_ptr = set_ASDU_to_frame(it, raw_buffer_ptr);
        }
    }
    return raw_buffer_ptr;
}

void SV_Frame::build(Attribute* atr_ptr){
        traverse(atr_ptr);
    }

void SV_Frame::traverse(Attribute* atr_ptr){
    atr_ptr->visit();
    for (auto it : atr_ptr->list_of_children) {
        std :: cout << "inside cycle "; atr_ptr->visit();
        traverse(it);
    }
}


/// временный метод ///
void SV_Frame:: print_frame(){
    std::cout << " FRAME " << std::endl;
    for (int i = 0; i < this->frame_size + 1; ++i) {
        std::cout << std::hex << " " << (int) raw_buffer[i] << " ";
        if (i%16 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;

}


SV_Frame::SV_Frame(General_SV_Settings sv_settings): EthernetFrame(sv_settings.eth_hdr, sv_settings.VLAN_ID, sv_settings.VLAN_Priority){

    attributes.savPdu = Container(0x60);
    attributes.seqASDU = Container(0xA2);
    attributes.ASDU = Container(0x30);
    /// инициализируем атрибуты asdu данными, указанными в настройках
    attributes.svID = String_attribute(sv_settings.asdu_parametrs.svID, 0x80);
    attributes.datSet = String_attribute(sv_settings.asdu_parametrs.datSet, 0x81);
    attributes.smpCnt = Type_attribute<uint16_t>(sv_settings.asdu_parametrs.smpCnt, 0x82);
    attributes.confrev = Type_attribute<uint32_t>(sv_settings.asdu_parametrs.confrev, 0x83);
    attributes.refrTm = Type_attribute<uint64_t>(sv_settings.asdu_parametrs.refrTm, 0x84);
    attributes.smpSynch = Type_attribute<uint8_t>(sv_settings.asdu_parametrs.smpSynch, 0x85);
    attributes.smpRate = Type_attribute<uint16_t>(sv_settings.asdu_parametrs.smpRate, 0x86);
    attributes.seq_of_Data = Seq_of_Data(0x87);
    attributes.noASDU = Type_attribute<uint32_t>(sv_settings.asdu_parametrs.noASDU, 0x80);

    build_savPdu();
    build_SV_frame(sv_settings);

}


/*
/// метод формирует атрибуты с заданным тегом и значениеями из настроектSV

void SV_Frame::initialize_ASDU_attributes(SV_attributes& asdu_attributes, ASDU_settings& source) {

    asdu_attributes.svID = String_attribute(source.svID, 0x80);
    asdu_attributes.datSet = String_attribute(source.datSet, 0x81);
    asdu_attributes.smpCnt = Type_attribute<uint16_t>(source.smpCnt, 0x82);
    asdu_attributes.confrev = Type_attribute<uint32_t>(source.confrev, 0x83);
    asdu_attributes.refrTm = Type_attribute<uint64_t>(source.refrTm, 0x84);
    asdu_attributes.smpSynch = Type_attribute<uint8_t>(source.smpSynch, 0x85);
    asdu_attributes.smpRate = Type_attribute<uint16_t>(source.smpRate, 0x86);
    asdu_attributes.seq_of_Data = Seq_of_Data(0x87);
    asdu_attributes.noASDU = Type_attribute<uint32_t>(source.noASDU, 0x80);
}

*/
