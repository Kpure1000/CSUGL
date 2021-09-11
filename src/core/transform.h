//
// Created by 匡涂上青 on 2021/7/4.
//

#ifndef CSUGL_TRANSFORM_H
#define CSUGL_TRANSFORM_H

#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"

namespace csugl {

    struct Transform {
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        Transform() = default;

        Transform(const glm::vec3 &position, const glm::quat &rotation, const glm::vec3 &scale)
                : position(position), rotation(rotation), scale(scale) {
        }

        inline glm::mat4 get_trans_mat() const {
            trans = glm::translate(glm::mat4(1.0f), position) * toMat4(rotation) * glm::scale(glm::mat4(1.0f), scale);
            return trans;
        }

        inline glm::mat4 get_inv_trans() const {
            inv_trans = glm::inverse(get_trans_mat());
            return inv_trans;
        }

    private:
        mutable glm::mat4 trans, inv_trans;

    };

}

#endif //CSUGL_TRANSFORM_H
