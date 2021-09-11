//
// Created by ktsq on 2021/8/22.
//

#ifndef CSUGL_MODEL_LOADER_H
#define CSUGL_MODEL_LOADER_H

#include "../core/core.h"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

//enum class aiTextureType;
struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace csugl::ml {

    struct MVec3 {
    public:
        union {
            float _v[3];
            struct {
                float x, y, z;
            };
        };

        MVec3() = default;

        MVec3(float x, float y, float z) : x(x), y(y), z(z) {}

        inline float operator[](int i) { return _v[i]; }
    };

    struct MVec2 {
        union {
            float _v[2];
            struct {
                float x, y;
            };
            struct {
                float u, v;
            };
        };

        MVec2() = default;

        MVec2(float x, float y) : x(x), y(y) {}

        inline float operator[](int i) { return _v[i]; }
    };

    enum class MTextureType {
        DIFFUSE,
        SPECULAR,
        AMBIENT,
        NORMAL,

        UNKNOWN
    };

    // Meta Texture
    struct MTexture {
        MTextureType type;
        int width, height, channel;
        static std::function<void(unsigned char *)> data_deleter;
        Scope<unsigned char, decltype(data_deleter)> data;
    };

    struct Mesh {
        Scope<MVec3[]> vertices;
        Scope<MVec3[]> normals;
        Scope<MVec2[]> uvs;
        Scope<MVec3[]> tangents;
        Scope<MVec3[]> biTangents;
        Scope<unsigned int[]> indices;
        unsigned int vertex_num;
        std::string name;
        std::vector<Ref<MTexture>> textures;
    };

    struct Model {
        std::string path;
        std::vector<Ref<Mesh>> meshes;
    };

    class model_loader {
    public:
        model_loader() = default;

        /**
         * Pre-load from file
         * @param fileName Name of file to load
         * @param modelName Name of model as key (= fileName default)
         */
        bool pre_load(const std::string &fileName, const std::string &modelName = "");

        /**
         * Load model from modules
         * @param modelName
         * @return
         */
        Ref<Model> load(const std::string &modelName);

    private:
        // [model_name, file_name]
        std::unordered_map<std::string, std::string > model_names;

        void InitMesh(const aiMesh *cur_mesh, const aiScene *scene, Model &newModel);

        void InitTexture(const aiMaterial *cur_mat, Mesh &newMesh, const Model &newModel);

        void GetTexture(const aiMaterial *cur_mat, unsigned int type, Mesh &newMesh, const std::string &dir);

    };

}

#ifndef SGT_MODEL_LOADER // singleton of model_loader
#define SGT_MODEL_LOADER() csugl::singleton<csugl::ml::model_loader>::getInstance()
#endif

#endif //CSUGL_MODEL_LOADER_H
