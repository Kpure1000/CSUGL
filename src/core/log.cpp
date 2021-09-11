//
// Created by 匡涂上青 on 2021/6/18.
//

#include <memory>
#include "log.h"

namespace csugl {
    Ref<spdlog::logger> Log::m_CoreLogger;

    Ref<spdlog::logger> Log::m_ClientLogger;

    void Log::Init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        m_CoreLogger = spdlog::stderr_color_mt("CSUGL");
        m_CoreLogger->set_level(spdlog::level::trace);

        m_ClientLogger = spdlog::stderr_color_mt("APP");
        m_ClientLogger->set_level(spdlog::level::trace);
    }
}