//
// Created by ktsq on 2021/8/6.
//

#include "input.h"

#include "application.h"
#include "window.h"
#include "../utils/singleton.h"

#include <GLFW/glfw3.h>

namespace csugl{

    bool Input::IsKeyPressed(KeyCode keyCode) {
        auto state = glfwGetKey(singleton<Application>::getInstance()->GetWindow().GetGLFWwindow(), keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(MouseCode mouseCode) {
        auto state = glfwGetMouseButton(singleton<Application>::getInstance()->GetWindow().GetGLFWwindow(), mouseCode);
        return state;
    }

    std::pair<float, float> Input::GetMousePos() {
        double x, y;
        glfwGetCursorPos(singleton<Application>::getInstance()->GetWindow().GetGLFWwindow(), &x, &y);
        return std::pair<float, float>(static_cast<float>(x), static_cast<float>(y));
    }

    float Input::GetMousePosX() {
        auto [x, y] = GetMousePos();
        return x;
    }

    float Input::GetMousePosY() {
        auto [x, y] = GetMousePos();
        return y;
    }

}
