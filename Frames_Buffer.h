//
// Created by amir on 12.04.23.
//

#ifndef SV_PROJECT_FRAMES_BUFFER_H
#define SV_PROJECT_FRAMES_BUFFER_H

#include "SV_Frame.h"
#include "CalcModule.h"
#include <stdint.h>
#include <queue>


/*!
 * Данный класс хранит буфер из 10 фреймов SV, готовых к отправке
 * По запросу от Sender могут быть предоставлены 1 или несколько фреймов.
 * В нормальном режиме за раз отправляется 1 фрейм, однако, если
 * Sender "проспал" более 250 мкс, может быть отправлено более 1 фрейма.
 * Например, если с прошлой отправки прошло около 450 мкс будет отправлено
 * 2 фрейма за раз.
 */
class Frames_Buffer{
public:

    u_char* pointers[10];

    std::queue<u_char* > frames_queue;

    /// Консторуктор
    Frames_Buffer(SV_Frame& svFrame, std::vector<CalcModule>& calc_values){

        uint32_t frame_size = svFrame.get_frame_size();
        u_char* frame_ptr = svFrame.get_raw_buffer();

        /// Создаем 10 копий фрейма, заполняем начальные значения фазных токов, напряжений
        for (int k = 0; k < 10; k++) {
            this->pointers[k] = new u_char[frame_size];
            memcpy(pointers[k], frame_ptr, frame_size);

            for (auto i: svFrame.seqOfData_ptr) {
                auto *seq_ptr = ( Seq_of_Data:: sequence_of_Data *)(pointers[k] + i);
                seq_ptr->Ia = calc_values[0].get_value_80();
                seq_ptr->Ib = calc_values[1].get_value_80();
                seq_ptr->Ic = calc_values[2].get_value_80();
                seq_ptr->In = calc_values[3].get_value_80();
                seq_ptr->Ua = calc_values[4].get_value_80();
                seq_ptr->Ub = calc_values[5].get_value_80();
                seq_ptr->Uc = calc_values[6].get_value_80();
                seq_ptr->Un = calc_values[7].get_value_80();
            }

            this->frames_queue.push(pointers[k]);
            frame_ptr = this->frames_queue.front();

        }

        ///////////// Разработать алгоритм изъятия и добавления фреймов




    }



public:

};



#endif //SV_PROJECT_FRAMES_BUFFER_H
