//
// Created by 匡涂上青 on 2021/6/18.
//

#ifndef CSUGL_CORE_H
#define CSUGL_CORE_H

#include <memory>

namespace csugl{
    template<typename T, typename Deleter = std::default_delete<T>>
    using Scope = std::unique_ptr<T, Deleter>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr Scope<T> MakeScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    constexpr Ref<T> MakeRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    unsigned int get_core_numbers();

}

#endif //CSUGL_CORE_H
