////
//// Created by amir on 30.12.22.
////
//
//#ifndef SV_PROJECT_SV_CLASSES_H
//#define SV_PROJECT_SV_CLASSES_H
//
//#endif //SV_PROJECT_SV_CLASSES_H
//
//#include <iostream>
//#include <list>
//#include <memory>
//#include <vector>
//#include <netinet/if_ether.h>
//#include <map>

//
//
//
//
//
///// настройки для потока
//struct ASDU_settings{
//    std::string svID = "ABCDEljkjkljj;ljljljljlkklgjhgjhgkjkFG';'l";   // 7 *
//    std::string datSet = "DATAS[op[o[pop[o[po[poET"; // 7
//    uint16_t smpCnt = 786;          // 2
//    uint32_t confrev = 665;         // 4
//    uint64_t refrTm = 999;          // 8
//    uint8_t smpSynch = 111;         // 1
//    uint16_t smpRate = 1;           // 2
//    uint32_t noASDU = 4;            // 4
//    instant_values val;
//};
//
/////  атрибуты пакета которым необходимо задавать значение
//struct SV_attributes{
//    String_attribute svID;
//    String_attribute datSet;
//    Type_attribute<uint16_t> smpCnt;
//    Type_attribute<uint32_t> confrev;
//    Type_attribute<uint64_t> refrTm;
//    Type_attribute<uint8_t> smpSynch;
//    Type_attribute<uint16_t> smpRate;
//    Seq_of_Data seq_of_Data;
//};
//
//struct SV_header{
//    uint16_t APPID = 0x4001;
//    uint16_t lengh = 0;
//    uint16_t reserved1 = 0;
//    uint16_t reserved2 = 0;
//};
//
//class SV_generator {
//public:
//
//    const std::map<int, std::string> sv_tags = {
//            {0x60, "savPdu"},
//            {0xA2, "seqASDU"},
//            {0x30, "ASDU"},
//            {0x80, "svID"},
//            {0x81, "datSet"},
//            {0x82, "smpCnt"},
//            {0x83, "confRev"},
//            {0x84, "refrTm"},
//            {0x85, "smpSynch"},
//            {0x86, "smpRate"},
//            {0x87, "seqData"}
//    };
//
//    struct ether_header settings {
//        .ether_dhost = {0x01, 0x0C, 0xCD, 0x04, 0x00, 0x01},
//        .ether_shost = {0xE8, 0x6C, 0xC7, 0x73, 0xA7, 0x1D},
//        .ether_type = 0xba88
//    };
//
//    //SV_header;
//    SV_attributes asdu_attr;
//
//    std::list<Attribute*> list_of_attribute;
//
//    //EthernetFrame frame = EthernetFrame(settings);
//
//    SV_header hdr;
//    Container savPdu = Container(0x60);
//    Container seqASDU = Container(0xA2);
//    Container ASDU = Container(0x30);
//
//    Type_attribute<uint32_t> noASDU;
//
//    SV_attributes initialize_ASDU(ASDU_settings& source);
//
//    Container build_SV_block(void);
//
//    /// методы
//
//    void build_frame();
//    u_char *traverse(Attribute *attr, u_char* frame_ptr);
//
//    SV_generator(){}
//
//    void print_frame();
//};
//
