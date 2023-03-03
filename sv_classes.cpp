//
// Created by amir on 01.01.23.
//

#include "sv_classes.h"
#include <cstring>



/// метод добавления элемента в контейнер сопровождается подсчетом объема данных

void Container:: add(Attribute& element) {
    if(element.length <= 128)                                   ///добавление метки не требуется
        this->length += element.length;
    else if (element.length > 128 && element.length <= 256) {     ///требуется добавление метки 0x81
        this->length += (element.length + 1);
        this->tag[1] = 0x81;
        this->tagSize = 2;
    }
    else if(element.length >= 256) {                              ///требуется добавление метки 0x82
        this->length += (element.length + 2);
        this->tag[1] = 0x82;
        this->tagSize = 2;
        this->lengthSize = 2;                                     /// значение длины поля value расположена в 2 байтах после метки 0x82
    }
    list_of_children.emplace_back(&element);

    std:: cout << "block " << std::hex << int (this->tag[0]) << std::dec << " now has size - " << this->length << ":: element size " << element.length << std::hex <<  " (added " << sv_tags.at(element.tag[0]) << " )" << std::endl;

}



// Задача: реализовать фнукцию, выполняющую создание структуры вложенных аттрибутов, инициализирующую поля
// в соответствии со значениями заданными в struct SV_attribute

///члены структуры asdu_attr инициализируются значениями из SV_attribute_values (где заданы настройки)
///также происходит присвоение тегов
SV_attributes SV_generator:: initialize_ASDU(SV_attribute_values& source){
    this->asdu_attr.svID = Simple_attribute(source.svID, 0x80);
    this->asdu_attr.datSet = Simple_attribute(source.datSet, 0x81);
    this->asdu_attr.smpCnt = TSimple_attribute<uint16_t>(source.smpCnt, 0x82);
    this->asdu_attr.confrev = TSimple_attribute<uint32_t>(source.confrev, 0x83);
    this->asdu_attr.refrTm = TSimple_attribute<uint64_t>(source.refrTm, 0x84);
    this->asdu_attr.smpSynch = TSimple_attribute<uint8_t>(source.smpSynch, 0x85);
    this->asdu_attr.smpRate = TSimple_attribute<uint16_t>(source.smpRate, 0x86);
    this->asdu_attr.seq_of_Data = Simple_attribute_seq(0x87);

    this->noASDU = TSimple_attribute<uint32_t>(source.noASDU, 0x80);

    return this->asdu_attr;
}

///сбоорка блока SV, сопровождается подсчетом размемора каждого блока-контейнера (length)
Container SV_generator::build_SV_block() {

///fill of ASDU
    this->ASDU.add(this->asdu_attr.svID);
    this->ASDU.add(this->asdu_attr.datSet);
    this->ASDU.add(this->asdu_attr.smpCnt);
    this->ASDU.add(this->asdu_attr.confrev);
    this->ASDU.add(this->asdu_attr.refrTm);
    this->ASDU.add(this->asdu_attr.smpSynch);
    this->ASDU.add(this->asdu_attr.smpRate);
    this->ASDU.add(this->asdu_attr.seq_of_Data);
///fill of seqASDU
    for (int i = 0; i < this->noASDU.value; i++) {
        this->seqASDU.add(this->ASDU);
    }
///fill of savPdu
    this->savPdu.add(this->noASDU);
    this->savPdu.add(this->seqASDU);

    this->hdr.lengh = this->savPdu.length;          /// задаем размера блока savPdu

    return this->savPdu;

}

/// после того как структура атрибутов создана и проинициализирована необходимо выполнить перенос
/// данных из стркутры данных(дерево атрибутов) в специальный буфер (масссив raw_buffer), для отправки через Ethernet (добавить данные в фрейм)
/// для этого необходимо корректно описать обход дерева

void SV_generator::build_frame() {
    std::cout << "\nStart of tree traverse\n"  << std::endl;
    this->traverse(&this->savPdu, frame.get_raw_ptr());
}

u_char* SV_generator::traverse(Attribute *attr, u_char* frame_ptr) {
    std::cout << "I AM IN " << this->sv_tags.at(attr->tag[0]) << std::endl;
    frame_ptr += attr->record_TLV(frame_ptr);                             /// определить класс ethernet frame

    if (attr->tag[0] & 0x20) {
        for (auto it: attr->list_of_children) {
            frame_ptr = traverse(it, frame_ptr);
        }
    }
    return frame_ptr;
}



/// временный метод ///
void SV_generator:: print_frame(){
    std::cout << " FRAME " << std::endl;
    for (int i = 0; i < savPdu.length + 14; ++i) {
        std::cout << std::hex << " " << (int) this->frame.raw_buffer[i] << " ";
        if (i%16 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;

}

