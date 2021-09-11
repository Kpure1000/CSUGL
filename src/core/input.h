//
// Created by ktsq on 2021/8/6.
//

#ifndef CSUGL_INPUT_H
#define CSUGL_INPUT_H

#include "core.h"

namespace csugl {

    using KeyCode = uint16_t;

    using MouseCode = uint16_t;

    class Input {
    public:
        static bool IsKeyPressed(KeyCode keyCode);
        static bool IsMouseButtonPressed(MouseCode mouseCode);
        static float GetMousePosX();
        static float GetMousePosY();
        static std::pair<float ,float> GetMousePos();
    };

}


#endif //CSUGL_INPUT_H
