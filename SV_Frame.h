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
///  Заголовок блока данных savPdu
struct SV_HDR{
    uint16_t APPID = 0x4001;
    uint16_t lengh = 0;
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
};
/// All parameters of Sampled Values data bloks
///  Структура объединяющая в себе все настройки для SV
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
    /// вектор хранит указатели на изменяющиеся параметры фрейма (smpCnt, smpSync и проч.)
    std::vector<uint32_t> smpSynch_ptr;
    std::vector<uint32_t> smpCnt_ptr;
    std::vector<uint32_t> seqOfData_ptr;

    uint32_t noASDU = 1;

    /// методы

    SV_Frame(){}
    /*!
     *  Конструктор принимает структуру General_SV_Settings, в которой перечислены все настройки для SV
     *  Внутри себя SV_Frame никаких параметров не хранит
     * @param sv_settings
     */
    SV_Frame(General_SV_Settings sv_settings);

    /*!
     * Метод заполнит "шапку" фрейма - поля перечисленные в struct SV_HDR
     * Также метод выполнит перенос данных из структуры вложенных атрибутов в готовый к отправке массив u_char
     * (через вызов mapping_savPDU_to_frame)
     */
    void build_SV_frame(struct SV_HDR& sv_header, struct savPdu_entries& attributes);

    /*!
     * Метод строит структуру вложенных атрибутов (дерево арибутов)
     * @param attributes
     */
    void build_savPdu(savPdu_entries& attributes);

    /*!
     * Рекурсивный обход дерева атрибутов и перенос данных в указанный массив
     * @param attribute_ptr - указатель на вершину дерева атрибутов (это атрибут savPDU, содержаший в себе все остальные атрибуты)
     * @param frame_ptr - указатель на массив (буфер для отправки в RAW_SOCKET)
     * @return
     */
    u_char* mapping_savPDU_to_frame(Attribute *attribute_ptr, u_char* frame_ptr);


    void print_frame();


    void traverse(Attribute* atr_ptr);
    void build (Attribute* atr_ptr);
};
