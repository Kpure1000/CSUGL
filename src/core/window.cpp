//
// Created by ktsq on 2021/6/19.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.h"
#include "event.h"
#include "window.h"

namespace csugl {

    static uint32_t m_GLFWWindowCount = 0;

    Window::Window(const WindowProps &props)
            : m_Data(MakeRef<WindowData>(props)) {
        if (m_GLFWWindowCount == 0) {
            int success = glfwInit();
            Log::AssertCore(success, "Could not initialize GLFW!");
            glfwSetErrorCallback([](int erCode, const char *description) {
                Log::ErrorCore("GLFW Error ({0}): {1}", erCode, description);
            });

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            m_GLFWWindowCount++;
        }

        {
            int monitorCount;
            auto monitor = glfwGetMonitors(&monitorCount);
            //  Use the first monitor in default
            m_CurMonitor = monitor[0];
            if (monitorCount > 0) {
                m_pVideoMode = (GLFWvidmode *) glfwGetVideoMode(monitor[0]);
                m_Monitors.reserve(monitorCount);
                m_Monitors.assign(monitor, monitor + monitorCount);
            }
        }

        {
            if (m_Data->isFullScreen) {
                m_Data->size.x = m_pVideoMode->width;
                m_Data->size.y = m_pVideoMode->height;
                m_Window = glfwCreateWindow(m_pVideoMode->width, m_pVideoMode->height, m_Data->title.c_str(),
                                            m_CurMonitor, NULL);
            } else {
                m_Window = glfwCreateWindow((int) m_Data->size.x, (int) m_Data->size.y, m_Data->title.c_str(),
                                            NULL, NULL);
            }
        }

        if (nullptr == m_Window) {
            Log::AssertCore(!m_Window, "Failed to create GLFW window");
            glfwTerminate();
        } else {

            glfwMakeContextCurrent(m_Window);
            Log::AssertCore((0 != gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)), "Failed to initialize GLAD");

            Log::InfoCore("\n>>>OpenGL Info>>>\n>Version:\t{0}\n>Vendor:\t{1}\n>Renderer:\t{2}\n>>>>>>>>>>>>>>>>>",
                          glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));

            // Set GLFW callbacks
            glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
                data.size.x = width;
                data.size.y = height;

                WindowResizeEvent event(width, height);
                for (auto &callback : data.eventCallBacks)
                    callback(event);
            });

            glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                for (auto &callback : data.eventCallBacks)
                    callback(event);
            });

            glfwSetWindowFocusCallback(m_Window, [](GLFWwindow *window, int isFocus) {
                if (GLFW_TRUE == isFocus) {
                    WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
                    WindowFocusEvent event;
                    for (auto &callback : data.eventCallBacks)
                        callback(event);
                } else {
                    WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
                    WindowFocusLostEvent event;
                    for (auto &callback : data.eventCallBacks)
                        callback(event);
                }
            });

            glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

                switch (action) {
                    case GLFW_PRESS: {
                        KeyPressedEvent event(key, 0);
                        for (auto &callback : data.eventCallBacks)
                            callback(event);
                        break;
                    }
                    case GLFW_RELEASE: {
                        KeyReleasedEvent event(key);
                        for (auto &callback : data.eventCallBacks)
                            callback(event);
                        break;
                    }
                    case GLFW_REPEAT: {
                        KeyPressedEvent event(key, 1);
                        for (auto &callback : data.eventCallBacks)
                            callback(event);
                    }
                    default:
                        break;
                }
            });

            glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int keycode) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

                KeyTypedEvent event(keycode);
                for (auto &callback : data.eventCallBacks)
                    callback(event);
            });

            glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

                switch (action) {
                    case GLFW_PRESS: {
                        MouseButtonPressedEvent event(button);
                        for (auto &callback : data.eventCallBacks)
                            callback(event);
                        break;
                    }
                    case GLFW_RELEASE: {
                        MouseButtonReleasedEvent event(button);
                        for (auto &callback : data.eventCallBacks)
                            callback(event);
                        break;
                    }
                }
            });

            glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

                MouseScrolledEvent event((float) xOffset, (float) yOffset);
                for (auto &callback : data.eventCallBacks)
                    callback(event);
            });

            glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

                MouseMovedEvent event((float) xPos, (float) yPos);
                for (auto &callback : data.eventCallBacks)
                    callback(event);
            });

            int getx, gety;
            glfwGetWindowSize(m_Window, &getx, &gety);
            m_Data->size = {getx, gety};

            glfwSetWindowUserPointer(m_Window, m_Data.get());

            glViewport(0, 0, (int) m_Data->size.x, (int) m_Data->size.y);

        }

        SetVSync(m_Data->VSync);
    }

    void Window::Clear(const glm::vec4 &clearColor) const {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::Display() const {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void Window::SetVSync(bool vsync) {
        if (vsync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data->VSync = vsync;
    }

    bool Window::isOpen() const {
        return !glfwWindowShouldClose(m_Window);
    }

    glm::ivec2 Window::GetScreenSize() const {
        return {this->m_pVideoMode->width, this->m_pVideoMode->height};
    }

}