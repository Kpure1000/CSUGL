//
// Created by 匡涂上青 on 2021/8/19.
//

#include "light.h"
#include "../core/transform.h"
#include "../core/camera.h"

namespace csugl {

    Light::Light(Transform &transform)
            : trans(transform) {
    }

    ////////////////////////////////
    /// Direction Light
    ////////////////////////////////

    DirectionLight::DirectionLight(Transform &transform)
            : Light(transform), caster(MakeScope<OrthographicCamera>(
            transform, glm::vec2{20.0f, 20.0f}, 1.0f, -1.0f, -20.0f)) {
    }

    void DirectionLight::render(const Shader &light_shader) const {

    }

    glm::mat4 DirectionLight::get_light_mvp() const {
        return caster->get_projection_mat() * caster->get_view_mat();
    }

}
