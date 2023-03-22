//
// Created by amir on 31.01.23.
//

#ifndef SV_PROJECT_SV_ATTRIBUTES_H
#define SV_PROJECT_SV_ATTRIBUTES_H

#endif //SV_PROJECT_SV_ATTRIBUTES_H


#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <cstring>
#include <byteswap.h>
#include <map>

///

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

/// структура хранит мгновенные значения токов и напряжений


/// абстрактный класс Attribute - родитель всех разновидностей атрибутов
class Attribute {
public:

    /// Поля
    uint8_t tag[2] = {0, 0};              ///поле tag хранит как тэг атрибута (в первом байте), так и, возможно, дополнительную метку 0х81 или 0х82 предварающую поле length
    uint16_t length{0};
    uint8_t tagSize{1};
    uint8_t lengthSize{1};
    std::list<Attribute *> list_of_children;

    /// Методы

    virtual u_char record_TLV(u_char *frame_ptr) = 0;
    void visit() {
        std::cout << " VISIT :: tag is " << std::hex << (int)this->tag[0] << std::endl;
        std::cout << " Size of list " << std::hex << (int)this->list_of_children.size() << std::endl;
    }

    /// Конструктор

    Attribute(uint8_t _tag) {
        this->tag[0] = _tag;
        tagSize = 1;
    }
    Attribute(){}

};
/// класс "контейнер", служит вместилищем для других атрибутов
class Container: public Attribute {
public:
    Container(uint8_t tag_): Attribute(tag_) {}
    Container( ) { }

    /// методы
    void add(Attribute& element);

    /// в классе контейнер, в буфер фрейма переносится только тэг и поле с длиной
    u_char record_TLV(u_char* frame_ptr) override {
        u_char shift = 0;
        uint16_t temp;
        memcpy(frame_ptr, &this->tag, this->tagSize);   std::cout << "\t\t" << std::hex << "tag " << int(this->tag[0]) <<" "<< int(this->tag[1]) << std::endl;
        shift+=tagSize;
        std::cout << "\t\t" << std::hex << "len " << int(this->length) << std::dec  << " = " << (this->length) << " tagSize " << int(this->tagSize) << " length size - " << (int)this->lengthSize << std::endl;
        if (lengthSize == 1){
            std:: cout << "TAG_SIZE = 1" << std::endl;
            memcpy(frame_ptr + shift, &this->length, this->lengthSize);
        }
        else{
            std:: cout << "TAG_SIZE = 2" << std::endl;
            memcpy(frame_ptr + shift, &(temp = bswap_16(this->length)), this->lengthSize);
        }
        shift+=this->lengthSize;
        return shift;
    };

};

/// шаблонный класс, для обычных атрибутов типа svID, smpCnt ("атрибут - не контейнер")
template <typename T>
class Type_attribute: public Attribute {
public:

    /// Конструктор
    /// инициализация через конструктор сопровождается определением размера данных

    Type_attribute(T &value, uint8_t _tag) : Attribute(
            _tag) {          ///перед созданием производного класса вызывается конструктор базового класса
        this->value = value;
        this->length = sizeof(this->value);

        if (this->length > 128 && this->length <= 256) {             ///требуется добавление метки 0x81
            this->tag[1] = 0x81;
            this->tagSize = 2;
        } else if (this->length >= 256) {                               ///требуется добавление метки 0x82
            this->tag[1] = 0x82;
            this->tagSize = 2;
        }

        //std::cout << "String_attribute( ):  " << " " << int(this->tag) << " " << this->value << " " << this->length << std::endl;
    }

    /// в буфер фрейма переносится тэг, длина и поле value
    u_char record_TLV(u_char *frame_ptr) override {

        u_char shift = 0;                                   /// на всякий случай определяем количество байт необходимое для хранения поля length
        memcpy(frame_ptr, &this->tag, this->tagSize);  std::cout << "\t\t" << std::hex << "tag " << int(this->tag[0]) <<" "<< int(this->tag[1]) << std::endl;      /// перенос тега в буфер
        shift += this->tagSize;                               /// перевод указателя на следующую ячейку
        memcpy(frame_ptr + shift, &this->length, this->lengthSize);             /// перенос поля length в буфер
        shift += this->lengthSize;                            std::cout << "\t\t" << std::hex << "len " << int(this->length) << std::dec  << " = " << (this->length) << " tagSize " << int(this->tagSize) << std::endl;      /// перевод указателя на следующую ячейку
        set_value(frame_ptr + shift);
        return shift += this->length;
    }


    Type_attribute() {}

    /// метод принимает указатель на байт в ethernet буфере и записывает по данному адресу value
    /// возвращается указатель на свободный блок памяти, следующий за значением value

    void set_value(u_char *frame_ptr) {
        T temp;

        switch (this->length) {
            case 1:
                memcpy(frame_ptr, &this->value, this->length);
                std::cout << "\t\t" << std::hex << "this->value : " << (int)this->value << " bswap(TATTRIBUTE) : one byte" << std::endl;
                std::cout << "\t\t" << std::hex << "frame_ptr : " << (int)*((T*)frame_ptr) <<  std::endl;
                break;
            case 2:
                memcpy(frame_ptr, &(temp = bswap_16(this->value)), this->length);
                std::cout << "\t\t" << std::hex << "this->value : " << this->value << " bswap 16 (TATTRIBUTE) : " << int(bswap_16(this->value)) << std::endl;
                std::cout << "\t\t" << std::hex << "frame_ptr : " << (int)*((T*)frame_ptr) <<  std::endl;
                break;
            case 4:
                memcpy(frame_ptr, &(temp = bswap_32(this->value)), this->length);
                std::cout << "\t\t" << std::hex << "this->value : " << this->value << " bswap 32 (TATTRIBUTE) : " << (int)(bswap_32(this->value)) << std::endl;
                std::cout << "\t\t" << std::hex << "frame_ptr : " << (int)*((T*)frame_ptr) <<  std::endl;
                break;
            case 8:
                memcpy(frame_ptr, &(temp = bswap_64(this->value)), this->length);
                std::cout << "\t\t" << std::hex << "this->value : " << this->value << " bswap 64 (TATTRIBUTE) : " << long(bswap_64(this->value)) << std::endl;
                std::cout << "\t\t" << std::hex << "frame_ptr : " << (long)*((T*)frame_ptr) <<  std::endl;
                break;
        }
    }

    T value;
};

class String_attribute: public Attribute{
public:
    /// Конструктор

    String_attribute(std::string& str, uint8_t _tag): Attribute(_tag) {  ///перед созданием производного класса вызывается конструктор базового класса
        this->value = str;
        this->length = str.size();
    std::cout << "string attr :: -> " << this->value <<" " << this->length << std::endl;
        std::cout << "c_str :: -> " << this->value.c_str() << std::endl;
        if (this->length > 128 && this->length <= 256) {             ///требуется добавление метки 0x81
            this->tag[1] = 0x81;
            this->tagSize = 2;
        }
        else if(this->length >= 256) {                               ///требуется добавление метки 0x82
            this->tag[1] = 0x82;
            this->tagSize = 2;
        }

    }

    /// Переопределение виртуального метода

    u_char record_TLV(u_char *frame_ptr) override {

        u_char shift = 0;                                   /// на всякий случай определяем количество байт необходимое для хранения поля length
        memcpy(frame_ptr, &this->tag, this->tagSize);        /// Копируем тег в буфер
        shift += tagSize;                               std::cout << "\t\t" << std::hex << "tag " << int(this->tag[0]) <<" "<< int(this->tag[1]) << std::endl; /// перевод указателя на следующую ячейку
        memcpy(frame_ptr + shift, &this->length, this->lengthSize);             /// Копируем поле length в буфер
        shift += this->lengthSize;                                 std::cout << "\t\t" << std::hex << "len " << int(this->length) << std::dec  << " = " << (this->length) << std::endl; /// перевод указателя на следующую ячейку
        memcpy(frame_ptr + shift, this->value.c_str(), this->value.size());      std::cout << "\t\t" << "set_value (STRING): " << (this->value.c_str()) << std::endl;
        shift += this->value.size();

        return shift;

    }



    size_t getsize() {return this->value.size();}

    String_attribute(){};


    std::string value;

};


class Seq_of_Data: public Attribute {
public:

    /// Поля

    struct instant_values {
        int32_t Ia{111}; int32_t Ia_Q{111};
        int32_t Ib{111}; int32_t Ib_Q{111};
        int32_t Ic{111}; int32_t Ic_Q{111};
        int32_t In{111}; int32_t In_Q{111};
        int32_t Ua{111}; int32_t Ua_Q{111};
        int32_t Ub{111}; int32_t Ub_Q{111};
        int32_t Uc{111}; int32_t Uc_Q{111};
        int32_t Un{111}; int32_t Un_Q{111};
    } values;




    /// Конструктор

    Seq_of_Data(uint8_t _tag): Attribute(_tag) {
        this->values = {0};
        this->length = sizeof(struct instant_values);
        this->values.Ia = 787;
    }

    Seq_of_Data(){}

    /// Методы

    /// Переопределение виртуального метода

    u_char record_TLV(u_char *frame_ptr) override {

        u_char shift = 0;
        memcpy(frame_ptr, &this->tag, this->tagSize);            /// Копируем тег в буфер
        shift += tagSize;                                  std::cout << "\t\t" << std::hex << "tag " << int(this->tag[0]) <<" "<< int(this->tag[1]) << std::endl;  /// перевод указателя на следующую ячейку
        memcpy(frame_ptr + shift, &this->length, this->lengthSize);                 /// Копируем поле length в буфер
        shift += this->lengthSize;                                    std::cout << "\t\t" << std::hex << "len " << int(this->length)<< std::dec  << " = " << (this->length) << std::endl;  /// перевод указателя на следующую ячейку
        set_value((struct instant_values*)(frame_ptr + shift));

        return shift += this->length;

    }

    void set_value(struct instant_values *frame_ptr) {

        frame_ptr->Ia = bswap_32(this->values.Ia);  frame_ptr->Ia_Q = bswap_32(this->values.Ia_Q);
        frame_ptr->Ib = bswap_32(this->values.Ib);  frame_ptr->Ib_Q = bswap_32(this->values.Ib_Q);
        frame_ptr->Ic = bswap_32(this->values.Ic);  frame_ptr->Ic_Q = bswap_32(this->values.Ic_Q);
        frame_ptr->In = bswap_32(this->values.In);  frame_ptr->In_Q = bswap_32(this->values.In_Q);
        frame_ptr->Ua = bswap_32(this->values.Ua);  frame_ptr->Ua_Q = bswap_32(this->values.Ua_Q);
        frame_ptr->Ub = bswap_32(this->values.Ub);  frame_ptr->Ub_Q = bswap_32(this->values.Ub_Q);
        frame_ptr->Uc = bswap_32(this->values.Uc);  frame_ptr->Uc_Q = bswap_32(this->values.Uc_Q);
        frame_ptr->Un = bswap_32(this->values.Un);  frame_ptr->Un_Q = bswap_32(this->values.Un_Q);

        std::cout << " SET VALUE - Ia\t\t" << std::hex << this->values.Ia << std::endl;
        std::cout << " SET VALUE - Ib\t\t" << std::hex << this->values.Ib << std::endl;
        std::cout << " SET VALUE - Ic\t\t" << std::hex << this->values.Ic << std::endl;
    }

};
