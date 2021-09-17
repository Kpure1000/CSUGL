//
// Created by 匡涂上青 on 2021/6/18.
//

#include "shader.h"
#include <core/core.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glog/logging.h>

namespace csugl {

    Shader::Shader(const std::string &filePath) {
#define INFO_BUFFER_SIZE 1024
#pragma region load shader code from file

        std::ifstream shaderIn;
        std::string shaderStr;
        shaderIn.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            shaderIn.open(filePath.c_str());
            std::stringstream vstream, fstream;
            vstream << shaderIn.rdbuf();
            shaderIn.close();
            shaderStr = vstream.str();
        }
        catch (std::ifstream::failure e) {
            LOG(ERROR) << "std::ifstream::failed: \n"
                       << e.what();
        }

#pragma endregion

#pragma region preprocess
        std::string vert_str;
        std::string frag_str;
        std::regex regex_vertex("(#type [\\s]*vertex)");        //  '#shader vertex'
        std::regex regex_fragment("(#type [\\s]*fragment)");    //  '#shader fragment'
        std::string pre_proc_str;

        //  split by '#shader vertex'
        auto sp_first = std::vector<std::string>(
                std::sregex_token_iterator(shaderStr.begin(), shaderStr.end(), regex_vertex, -1),
                std::sregex_token_iterator());
        std::vector<std::string> sp_second;
        if (!sp_first.empty()) //  found '#type vertex'
        {
            pre_proc_str = (*(sp_first.end() - 1)); //  get the latter
            //  split by '#type fragment'
            sp_second = std::vector<std::string>(
                    std::sregex_token_iterator(pre_proc_str.begin(), pre_proc_str.end(), regex_fragment, -1),
                    std::sregex_token_iterator());
            if (sp_second.size() > 1) //  vertex code is the former
            {
                vert_str = sp_second[0];
                frag_str = sp_second[1];
            } else {
                //  split by '#shader fragment'
                sp_first = std::vector<std::string>(
                        std::sregex_token_iterator(shaderStr.begin(), shaderStr.end(), regex_fragment, -1),
                        std::sregex_token_iterator());
                if (!sp_first.empty()) {
                    pre_proc_str = (*(sp_first.end() - 1)); //  get the latter
                    //  split by '#shader vertex'
                    sp_second = std::vector<std::string>(
                            std::sregex_token_iterator(pre_proc_str.begin(), pre_proc_str.end(), regex_vertex, -1),
                            std::sregex_token_iterator());
                    if (sp_second.size() > 1) //  fragment code is the former
                    {
                        frag_str = sp_second[0];
                        vert_str = sp_second[1];
                    }
                }
            }
        }

#pragma endregion

#pragma region vertex shader

        int isCompileSuccessed;

        //  create vertex shader
        unsigned int vShader;
        vShader = glCreateShader(GL_VERTEX_SHADER);
        //  load shader code
        const char *vShaderCode = vert_str.c_str();
        glShaderSource(vShader, 1, &vShaderCode, NULL);
        //  compile code
        glCompileShader(vShader);
        char infoLog[INFO_BUFFER_SIZE];
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &isCompileSuccessed);
        if (!isCompileSuccessed) {
            glGetShaderInfoLog(vShader, INFO_BUFFER_SIZE, NULL, infoLog);
            LOG(ERROR) << "Vertex shader compilation failed, in file:\'" << filePath << "\'. Details:\n"
                       << infoLog;
        }

#pragma endregion

#pragma region fragment shader

        //  cerate fragment shader
        unsigned int fShader;
        fShader = glCreateShader(GL_FRAGMENT_SHADER);
        //  load shader code
        const char *fShaderCode = frag_str.c_str();
        glShaderSource(fShader, 1, &fShaderCode, NULL);
        //  compile
        glCompileShader(fShader);
        char finfoLog[INFO_BUFFER_SIZE];
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &isCompileSuccessed);
        if (!isCompileSuccessed) {
            glGetShaderInfoLog(fShader, INFO_BUFFER_SIZE, NULL, finfoLog);
            LOG(ERROR) << "Fragment shader compilation failed, in file:\'" << filePath << "\'. Details:\n"
                       << finfoLog;
        }

#pragma endregion

#pragma region shader program

        m_ID = glCreateProgram();
        //  add shader to program
        glAttachShader(m_ID, vShader);
        glAttachShader(m_ID, fShader);
        //  link shaders from program
        glLinkProgram(m_ID);

        char pinfoLog[INFO_BUFFER_SIZE];
        glGetProgramiv(m_ID, GL_LINK_STATUS, &isCompileSuccessed);
        if (!isCompileSuccessed) {
            glGetProgramInfoLog(m_ID, INFO_BUFFER_SIZE, NULL, pinfoLog);
            LOG(ERROR) << "Shader program link failed. Details:\n" << pinfoLog;
        }

        //  release shaders
        glDeleteShader(vShader);
        glDeleteShader(fShader);

#pragma endregion

#pragma region get shader name from path

        // split by '/' or '\'
        std::regex regex_div_slash(R"(\\|\/)");
        std::vector<std::string> split_div_slash(
                std::sregex_token_iterator(filePath.begin(), filePath.end(), regex_div_slash, -1),
                std::sregex_token_iterator()
        );
        std::regex regex_dot("\\.");
        if (!split_div_slash.empty()) {
            auto &slashed = split_div_slash[split_div_slash.size() - 1];
            std::vector<std::string> split_dot(
                    std::sregex_token_iterator(slashed.begin(), slashed.end(), regex_dot, -1),
                    std::sregex_token_iterator()
            );
            if (!split_dot.empty()) {
                m_Name = split_dot[0];
            } else {
                m_Name = "";
            }
        } else {
            m_Name = "";
        }

#pragma endregion

#undef INFO_BUFFER_SIZE
    }

    Shader::~Shader() {
        glDeleteProgram(m_ID);
    }

    void Shader::Use() const {
        glUseProgram(m_ID);
    }

    void Shader::UnUse() const {
        glUseProgram(0);
    }

    void Shader::SetBool(const std::string &name,
                         const bool &value) const {
        SetInt(name, value);
    }

    void Shader::SetInt(const std::string &name,
                        const int &value) const {
        glUniform1i(GetPropertyID(name), value);
    }

    void Shader::SetFloat(const std::string &name,
                          const float &value) const {
        glUniform1f(GetPropertyID(name), value);
    }

    void Shader::SetVector2(const std::string &name,
                            const glm::vec2 &value) const {
        glUniform2f(GetPropertyID(name), value.x, value.y);
    }

    void Shader::SetVector3(const std::string &name,
                            const glm::vec3 &value) const {
        glUniform3f(GetPropertyID(name), value.x, value.y, value.z);
    }

    void Shader::SetVector4(const std::string &name,
                            const glm::vec4 &value) const {
        glUniform4f(GetPropertyID(name), value.x, value.y, value.z, value.w);
    }

    void Shader::SetRGBA(const std::string &name, const glm::vec4 &color) const {
        SetVector4(name, color);
    }

    void Shader::SetRGB(const std::string &name, const glm::vec3 &color) const {
        SetVector3(name, glm::vec3(color[0], color[1], color[2]));
    }

    void Shader::SetMatrix4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(GetPropertyID(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void Shader::SetSampler2D(const std::string &name, const int &index) const {
        glUniform1i(GetPropertyID(name), index);
    }

    void Shader::SetFloatArray(const std::string &name, const int &size, const float *data) const {
        glUniform1fv(GetPropertyID(name), size, data);
    }

    void Shader::SetIntArray(const std::string &name, const int &size, const int *data) const {
        glUniform1iv(GetPropertyID(name), size, data);
    }

    int Shader::GetPropertyID(const std::string &name) const {
        return glGetUniformLocation(m_ID, (name).c_str());
    }

}
