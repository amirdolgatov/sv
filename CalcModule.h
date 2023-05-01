//
// Created by amir on 18.04.23.
//

#ifndef SV_PROJECT_CALCMODULE_H
#define SV_PROJECT_CALCMODULE_H

#include <stdint.h>

enum HarmonicIndex{
    Ampliude = 0,
    Fase     = 1
};

class CalcModule{
public:
    ///  Хранение амплитуды и фазы для каждой гармоники
    float harmonics[10][2];

    int32_t values_80[80]{0};         /// массив с рассчитанными точками

    int32_t values_256[256]{0};       /// массив с рассчитанными точками

    /// Счетчик хранит номер текущей точки
    int32_t counter_80 = 0;
    int32_t counter_256 = 0;

    /// Задать амплиту и фазовый сдвиг гармоникам
    void set_harmonic(uint8_t index, float value, float fase){
        this->harmonics[index][HarmonicIndex::Ampliude] = value;
        this->harmonics[index][HarmonicIndex::Fase] = fase;
    }
    /// получить очередную точку синусоиды 80 точек/пер
    int32_t get_value_80();
    /// получить очередную точку синусоиды 256 точек/пер
    int32_t get_value_256();

    ///  Заполнить массив значениями для 80 точек/пер
    void calculate_80();
    ///  Заполнить массив значениями для 256 точек/пер
    void calculate_256();

};


#endif //SV_PROJECT_CALCMODULE_H
