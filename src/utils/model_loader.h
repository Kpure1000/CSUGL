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

    struct MMaterial {
        float Ka[3];
        float Kd[3];
        float Ks[3];
        Ref<MTexture> map_Ka;
        Ref<MTexture> map_Kd;
        Ref<MTexture> map_Ks;
    };

    // Meta Mesh
    struct MMesh {
        Scope<float[]> vertices;    // vec3
        Scope<float[]> normals;     // vec3
        Scope<float[]> uvs;         // vec2
        Scope<float[]> tangents;    // vec3
        Scope<float[]> biTangents;  // vec3
        Scope<unsigned int[]> indices;
        unsigned int vertex_num;
        unsigned int indices_num;
        std::string name;
        // std::vector<Ref<MTexture>> textures;
        Ref<MMaterial> material;
    };

    struct MModel {
        std::string path;
        std::vector<Ref<MMesh>> meshes;
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
        Ref<MModel> load(const std::string &modelName);

    private:
        // [model_name, file_name]
        std::unordered_map<std::string, std::string > model_names;

        void InitMesh(const aiMesh *cur_mesh, const aiScene *scene, MModel &newModel);

        void InitTexture(const aiMaterial *cur_mat, MMesh &newMesh, const MModel &newModel);

        void GetTexture(const aiMaterial *cur_mat, unsigned int type, MMesh &newMesh, const std::string &dir);

    };

}

#ifndef SGT_MODEL_LOADER // singleton of model_loader
#define SGT_MODEL_LOADER() csugl::singleton<csugl::ml::model_loader>::getInstance()
#endif

#endif //CSUGL_MODEL_LOADER_H
