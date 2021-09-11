//
// Created by ktsq on 2021/8/6.
//

#include "application.h"

#include "window.h"
#include "event.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace csugl {

    Application::Application() : isRunning(false) {
        glm::ivec2 winSize(800, 600);
        window = csugl::MakeScope<csugl::Window>(csugl::Window(
                csugl::WindowProps("CSUGL Engine", winSize.x, winSize.y, true, true)
        ));
        isRunning = true;
        window->addEventCallback([this](csugl::Event &ev) {
            csugl::EventDispatcher dispatcher(ev);
            dispatcher.Dispatch<csugl::WindowCloseEvent>([this](csugl::WindowCloseEvent &ev) {
                isRunning = false;
                return false;
            });
            dispatcher.Dispatch<csugl::WindowResizeEvent>([](csugl::WindowResizeEvent &ev) {
                glViewport(0, 0, ev.width, ev.height);
                return false;
            });
            dispatcher.Dispatch<csugl::KeyPressedEvent>([this](csugl::KeyPressedEvent &ev) {
                if (ev.keyCode == csugl::Key::Escape)
                    isRunning = false;
                return false;
            });
        });
    }

    Window &Application::GetWindow() const {
        return *window;
    }

    bool Application::isOpen() const {
        return isRunning;
    }

}
