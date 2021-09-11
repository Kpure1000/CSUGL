//
// Created by 匡涂上青 on 2021/6/18.
//

#ifndef CSUGL_SHADER_H
#define CSUGL_SHADER_H

#include "../vendor/glm/glm/glm.hpp"
#include <string>

namespace csugl {
    class Shader {
    public:
        Shader(const std::string &filePath);

        ~Shader();

        void Use() const;

        void UnUse() const;

        [[nodiscard]] const std::string &GetName() const { return m_Name; }

        void SetName(const std::string &name) { m_Name = name; }

    public:
        void SetBool(const std::string &name, const bool &value) const;

        void SetInt(const std::string &name, const int &value) const;

        void SetFloat(const std::string &name, const float &value) const;

        void SetVector2(const std::string &name, const glm::vec2 &value) const;

        void SetVector3(const std::string &name, const glm::vec3 &value) const;

        void SetVector4(const std::string &name, const glm::vec4 &value) const;

        void SetRGBA(const std::string &name, const glm::vec4 &color) const;

        void SetRGB(const std::string &name, const glm::vec3 &color) const;

        void SetMatrix4(const std::string &name, const glm::mat4 &mat) const;

        void SetSampler2D(const std::string &name, const int &index) const;

        void SetFloatArray(const std::string &name, const int &size, const float *data) const;

        void SetIntArray(const std::string &name, const int &size, const int *data) const;

    private:
        /// <summary>
        /// Get property ID by shaderID and property name
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        [[nodiscard]] int GetPropertyID(const std::string &name) const;

    private:
        /// <summary>
        /// id of shader program
        /// </summary>
        unsigned int m_ID;

        std::string m_Name;
    };
}


#endif //CSUGL_SHADER_H
