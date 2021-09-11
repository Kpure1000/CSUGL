//
// Created by 匡涂上青 on 2021/8/19.
//

#ifndef CSUGL_LIGHT_H
#define CSUGL_LIGHT_H

#include "../core/core.h"

#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"

namespace csugl {

    struct Transform;

    class OrthographicCamera;
    class Shader;

    class Light {
    public:
        explicit Light(Transform &transform);

        virtual void render(const Shader& light_shader)const = 0;

        [[nodiscard]] virtual glm::mat4 get_light_mvp() const = 0;

    private:
        Transform &trans;
    };

    class DirectionLight : public Light {
    public:
        explicit DirectionLight(Transform &transform);

        void render(const Shader& light_shader)const override;

        [[nodiscard]] glm::mat4 get_light_mvp() const override;

    private:
        mutable Scope<OrthographicCamera> caster;
    };

}


#endif //CSUGL_LIGHT_H
