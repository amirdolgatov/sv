//
// Created by amir on 06.03.23.
//

#include "SV_Frame.h"


void SV_Frame::build_SV_frame(struct SV_HDR& sv_header, struct savPdu_entries& attributes) {
    /// сформируем блок savPdu
    /// set  fields APPID, length, reserved1, reserved2

    struct SV_HDR *pointer = (struct SV_HDR *) this->frame_payload;

    pointer->APPID = bswap_16(sv_header.APPID);
    pointer->lengh = bswap_16( attributes.savPdu.length + attributes.savPdu.tagSize + attributes.savPdu.lengthSize + 8);
    pointer->reserved1 = bswap_16(sv_header.reserved1);
    pointer->reserved2 = bswap_16(sv_header.reserved2);

    this->frame_size += sizeof(struct SV_HDR);

    mapping_savPDU_to_frame(&attributes.savPdu,(u_char*)pointer + sizeof(struct SV_HDR));

    /// определяем конечный размер фрейма
    this->frame_size += (attributes.savPdu.length + attributes.savPdu.tagSize + attributes.savPdu.lengthSize);

}

void SV_Frame::build_savPdu(savPdu_entries& attributes) {

    /// начало формирования структуры (дерева) savPdu
    ///fill of ASDU
    attributes.ASDU.add(attributes.svID);
    attributes.ASDU.add(attributes.datSet);
    attributes.ASDU.add(attributes.smpCnt);
    attributes.ASDU.add(attributes.confrev);
    attributes.ASDU.add(attributes.refrTm);
    attributes.ASDU.add(attributes.smpSynch);
    attributes.ASDU.add(attributes.smpRate);
    attributes.ASDU.add(attributes.seq_of_Data);
    ///fill of seqASDU
    for (int i = 0; i < attributes.noASDU.value; i++) {
        attributes.seqASDU.add(attributes.ASDU);
    }
    ///fill of savPdu
    attributes.savPdu.add(attributes.noASDU);
    attributes.savPdu.add(attributes.seqASDU);

}

/// mapping_savPDU_to_frame - перенос значений из дерева атрибутов (коим является объект savPdu) в массив u_char, для последующей отправки через сокет

/// !!! Перенос данных реализован через обход графа, состоящего из вложенных друг в друга атрибутов (объекты - наследники класса Attribute).
/// !!! Вместе с переносом данных происходит сохранение адресов для тех параметров (атрибутов), значения которых будут изменияться, например
/// !!! поля smpCnt, smpSynch, мгновенные фазные значения.
u_char* SV_Frame::mapping_savPDU_to_frame(Attribute *attribute_ptr, u_char* raw_buffer_ptr) {

    /// Если это поле smpCnt, smpSynch или seqOfData, то необходимо его сохранить адрес относительно начала массива u_char
    if (attribute_ptr->tag[0] == 0x82)
        this->smpCnt_ptr.emplace_back(raw_buffer_ptr - this->get_raw_buffer());
    else if (attribute_ptr->tag[0] == 0x85)
        this->smpSynch_ptr.emplace_back(raw_buffer_ptr - this->get_raw_buffer());
    else if (attribute_ptr->tag[0] == 0x87)
        this->seqOfData_ptr.emplace_back(raw_buffer_ptr - this->get_raw_buffer());

    /// перенести данные в буфер, сдвинуть указатель вперед, на величину записанных данных
    raw_buffer_ptr += attribute_ptr->record_TLV(raw_buffer_ptr);

    if (attribute_ptr->tag[0] & 0x20) {
        for (auto it: attribute_ptr->list_of_children) {
            raw_buffer_ptr = mapping_savPDU_to_frame(it, raw_buffer_ptr);
        }
    }
    return raw_buffer_ptr;
}



/// Результатом работы конструктора будет готовый отправке массив u_char
SV_Frame::SV_Frame(General_SV_Settings sv_settings): EthernetFrame(sv_settings.eth_hdr, sv_settings.VLAN_ID, sv_settings.VLAN_Priority){

    savPdu_entries attributes;

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

    this->noASDU = attributes.noASDU.value;

    build_savPdu(attributes);
    build_SV_frame(sv_settings.sv_header, attributes);



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



//void SV_Frame::build(Attribute* atr_ptr){
//        traverse(atr_ptr);
//    }
//
//void SV_Frame::traverse(Attribute* atr_ptr){
//    atr_ptr->visit();
//    for (auto it : atr_ptr->list_of_children) {
//        std :: cout << "inside cycle "; atr_ptr->visit();
//        traverse(it);
//    }
//}
//
//
///// временный метод ///
//void SV_Frame:: print_frame(){
//    std::cout << " FRAME " << std::endl;
//    for (int i = 0; i < this->frame_size + 1; ++i) {
//        std::cout << std::hex << " " << (int) raw_buffer[i] << " ";
//        if (i%16 == 0)
//            std::cout << std::endl;
//    }
//    std::cout << std::endl;
//
//}
