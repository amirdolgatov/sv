//
// Created by amir on 18.04.23.
//

#include "CalcModule.h"
#include <math.h>

/*
 *  Используются первые 10 гармоник, 50 Гц - основная гармоника
 */

///  Заполнить массив значениями для 80 точек/пер
void CalcModule::calculate_80() {
    for (int i = 0; i < 80; i++) {
        for (int k = 1; k <= 10; k++) {
            this->values_80[i] += this->harmonics[k - 1][Ampliude] * sin(i * 2 * M_PI / (80 * k) + harmonics[k - 1][Fase]);
        }
        this->values_80[i] = static_cast<int32_t>(this->values_80[i]);
    }
}

///  Заполнить массив значениями для 256 точек/пер
void CalcModule::calculate_256() {
    for (int i = 0; i < 256; i++) {
        for (int k = 1; k <= 10; k++) {
            this->values_256[i] += this->harmonics[k - 1][Ampliude] * sin(i * 2 * M_PI/(256 * k) + + harmonics[k - 1][Fase]);
        }
        this->values_256[i] = static_cast<int32_t>(this->values_256[i]);
    }
}

/// получить очередную точку синусоиды 80 точек/пер
int32_t CalcModule::get_value_80(){
    this->counter_80 = (++this->counter_80)% 80;
    return this->values_80[this->counter_80 - 1];
}
/// получить очередную точку синусоиды 256 точек/пер
int32_t CalcModule::get_value_256(){
    this->counter_256 = (++this->counter_256)% 256;
    return this->values_256[this->counter_256 - 1];
}
