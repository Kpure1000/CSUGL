//
// Created by 匡涂上青 on 2021/7/5.
//

#ifndef CSUGL_CAMERA_H
#define CSUGL_CAMERA_H

#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/matrix_transform.hpp"

namespace csugl {

    struct Transform;

    class Camera {
    public:
        explicit Camera(Transform& transform);

        glm::mat4 get_view_mat() const;

        virtual glm::mat4 get_projection_mat() const = 0;

        Transform& transform;
    };

    class OrthographicCamera : public Camera {
    public:
        OrthographicCamera(Transform& transform, const glm::vec2 &size, float zoom, float _near, float _far);

        glm::mat4 get_projection_mat() const override;

        glm::vec2 size;
        float _near, _far;
    };

    class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera(Transform& transform, float fov, float aspect, float _near, float _far);

        glm::mat4 get_projection_mat() const override;

        float fov, aspect, _near, _far;
    };

}

#endif //CSUGL_CAMERA_H
