//
// Created by ktsq on 2021/8/7.
//

#include "camera.h"
#include "transform.h"

namespace csugl {

    Camera::Camera(Transform &transform) : transform(transform) {
    }

    glm::mat4 Camera::get_view_mat() const {
        glm::vec3 front = glm::rotate(transform.rotation, {0, 0, -1.0f});
        glm::vec3 up(0, 1.0f, 0);
        return glm::lookAt(
                transform.position,
                transform.position + front,
                glm::cross(glm::cross(front, up), front)
        );
    }

    OrthographicCamera::OrthographicCamera(Transform &transform, const glm::vec2 &size, float zoom, float _near,
                                           float _far)
            : Camera(transform), size(size), _near(_near), _far(_far) {
    }

    glm::mat4 OrthographicCamera::get_projection_mat() const {
        return glm::ortho(-size.x * 0.5f, size.x * 0.5f, -size.y * 0.5f, size.y * 0.5f, _near, _far);
    }

    PerspectiveCamera::PerspectiveCamera(Transform &transform, float fov, float aspect, float _near, float _far)
            : Camera(transform), fov(fov), aspect(aspect), _near(_near), _far(_far) {
    }

    glm::mat4 PerspectiveCamera::get_projection_mat() const {
        return glm::perspective(fov, aspect, _near, _far);
    }

}