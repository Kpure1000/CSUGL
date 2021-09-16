//
// Created by ktsq on 2021/6/19.
//

#ifndef CSUGL_WINDOW_H
#define CSUGL_WINDOW_H

#include "../vendor/glm/glm/glm.hpp"

#include <vector>
#include <functional>
#include <string>

struct GLFWwindow;
struct GLFWvidmode;
struct GLFWmonitor;

namespace csugl {

    class Event;

    struct WindowProps {
        //  Window title
        std::string title;

        //  Window size
        glm::vec2 size;

        //  Is window fullscreen
        bool isFullScreen;

        bool isVSync;

        WindowProps(const std::string &Title = "CSUGL Engine",
                    uint32_t Width = 800, uint32_t Height = 600, bool VSync = false, bool FullScreen = false)
                : title(Title), size(Width, Height), isVSync(VSync), isFullScreen(FullScreen) {}

    };

    class Window {

    private:
        GLFWwindow *m_Window;
        std::vector<GLFWmonitor *> m_Monitors;
        GLFWmonitor * m_CurMonitor;
        GLFWvidmode *m_pVideoMode;

        using EventCallbackFn = std::function<void(Event &)>;

        struct WindowData {
            std::string title;
            glm::vec2 size{};
            bool VSync = false;
            bool isFullScreen = false;

            std::vector<EventCallbackFn> eventCallBacks;

            explicit WindowData(const WindowProps &props) {
                title = props.title;
                size = props.size;
                VSync = props.isVSync;
                isFullScreen = props.isFullScreen;
            }
        };

        Ref<WindowData> m_Data;

    public:
        explicit Window(const WindowProps &props);

        inline void addEventCallback(const EventCallbackFn &callback) {
            m_Data->eventCallBacks.push_back(callback);
        }

        glm::ivec2 GetSize() const { return m_Data->size; }

        glm::ivec2 GetScreenSize() const;

        void Display() const;

        void SetVSync(bool vsync);

        GLFWwindow *GetGLFWwindow() const { return m_Window; }

        [[nodiscard]] bool isOpen() const;

    };
}

#endif //CSUGL_WINDOW_H
