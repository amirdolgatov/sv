//
// Created by amir on 12.04.23.
//

#ifndef SV_PROJECT_FRAMES_BUFFER_H
#define SV_PROJECT_FRAMES_BUFFER_H

#endif //SV_PROJECT_FRAMES_BUFFER_H

#include "SV_Frame.h"
#include <queue>

class Frames_Buffer{

public:
    std::queue<SV_Frame> frames;

    uint8_t maxsize = 6;

    SV_Frame svFrame;

    int get_size(){
        return frames.size();
    }

    Frames_Buffer(SV_Frame& _svFrame){
        this->svFrame = _svFrame;
    };
};
