//
// Created by ktsq on 2021/8/6.
//

#ifndef CSUGL_APPLICATION_H
#define CSUGL_APPLICATION_H

#include "core.h"

namespace csugl {

    struct WindowProps;

    class Window;

    class Application {
    public:
        explicit Application();

        ~Application();

        Window &GetWindow() const;

        bool isOpen() const;

        void close() { isRunning = false; }

    private:
        bool isRunning;
        Scope<Window> window;
    };

}


#endif //CSUGL_APPLICATION_H
