//
// Created by ktsq on 2021/8/22.
//

#include "model_loader.h"
#include "../core/log.h"
#include "../core/file_system.h"
#include "asset_manager.h"
#include "singleton.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <memory>

namespace csugl::ml {

    std::function<void(unsigned char *)> MTexture::data_deleter = [](unsigned char *img_data) {
        stbi_image_free(img_data);
    };

    bool model_loader::pre_load(const std::string &fileName, const std::string &modelName) {
        auto realModelName = modelName.empty() ? fs::get_file_name(fileName) : modelName;
        if (model_names.find(modelName) != model_names.end()) {
            csugl::Log::Warn("Model '{0}' existed at '{1}'", modelName, model_names[modelName]);
            return true;
        }
        auto models_am = SGT_Ref_Str_AM(Model);
        auto oldModel = models_am->get(fileName);
        // model exist
        if (oldModel) {
            csugl::Log::Warn("Model existed at '{0}', add model name '{1}'", fileName, realModelName);
            // add [model_name, file_name]
            model_names.try_emplace(realModelName, fileName);
            return true;
        }
        Assimp::Importer importer;
        auto scene = importer.ReadFile(fileName,
                                       aiProcess_CalcTangentSpace |
                                       aiProcess_Triangulate |
                                       aiProcess_JoinIdenticalVertices |
                                       aiProcess_FlipUVs | // flip uvs in OpenGL
                                       aiProcess_SortByPType);

        if (!scene) {
            csugl::Log::Error("Assimp: file '{0}' error info: {1}", fileName, importer.GetErrorString());
            return false;
        }

        if (!scene->HasMeshes()) {
            csugl::Log::Error("Assimp: there is no mesh in {0}", fileName);
            return false;
        }

        // Init mesh
        auto newModel = MakeRef<Model>();
        newModel->path = fileName;

        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            const auto cur_mesh = scene->mMeshes[i];
            InitMesh(cur_mesh, scene, *newModel);
        }

        if (newModel->meshes.empty()) {
            csugl::Log::Error("Assimp: Model '{0}' at '{1}' load failed with empty mesh", realModelName, fileName);
            return false;
        }
        models_am->add(fileName, newModel);
        model_names.try_emplace(realModelName, fileName);
        return true;
    }

    Ref<Model> model_loader::load(const std::string &modelName) {
        if (model_names.find(modelName) != model_names.end()) {
            return SGT_Ref_Str_AM(Model)->get(model_names[modelName]);
        }
        return nullptr;
    }

    void model_loader::InitMesh(const aiMesh *cur_mesh, const aiScene *scene, Model &newModel) {
        auto newMesh = MakeRef<Mesh>();
        if (!cur_mesh->HasPositions() || !cur_mesh->HasNormals()) {
            csugl::Log::Error("Assimp: no vertices or normal info in mesh '{0}'", std::string(cur_mesh->mName.data, cur_mesh->mName.length));
            return;
        }
        newMesh->name = std::string(cur_mesh->mName.data, cur_mesh->mName.length);
        newMesh->vertex_num = cur_mesh->mNumVertices;
        newMesh->vertices = MakeScope<MVec3[]>(cur_mesh->mNumVertices);
        newMesh->normals = MakeScope<MVec3[]>(cur_mesh->mNumVertices);
        newMesh->tangents = MakeScope<MVec3[]>(cur_mesh->mNumVertices);
        newMesh->biTangents = MakeScope<MVec3[]>(cur_mesh->mNumVertices);
        newMesh->uvs = MakeScope<MVec2[]>(cur_mesh->mNumVertices);

#define GET_POS_NOR_TAN_BTAN(i) newMesh->vertices[i] = {cur_mesh->mVertices[i].x, cur_mesh->mVertices[i].y, cur_mesh->mVertices[i].z}; \
                newMesh->normals[i] = {cur_mesh->mNormals[i].x, cur_mesh->mNormals[i].y, cur_mesh->mNormals[i].z}; \
                newMesh->tangents[i] = {cur_mesh->mTangents[i].x, cur_mesh->mTangents[i].y, cur_mesh->mTangents[i].z}; \
                newMesh->biTangents[i] = {cur_mesh->mBitangents[i].x, cur_mesh->mBitangents[i].y, cur_mesh->mBitangents[i].z};

        if (cur_mesh->HasTextureCoords(0)) {
            for (unsigned int i = 0; i < cur_mesh->mNumVertices; i++) {
                GET_POS_NOR_TAN_BTAN(i)
                newMesh->uvs[i] = {cur_mesh->mTextureCoords[0][i].x, cur_mesh->mTextureCoords[0][i].y};
            }
        } else {
            csugl::Log::Warn("Assimp: no vertices or normal info in mesh '{0}'", cur_mesh->mName.C_Str());
            for (unsigned int i = 0; i < cur_mesh->mNumVertices; i++) {
                GET_POS_NOR_TAN_BTAN(i)
                newMesh->uvs[i] = {0.0f, 0.0f};
            }
        }
#undef GET_POS_NOR_TAN_BTAN

        // load face indices
        newMesh->indices = MakeScope<unsigned int[]>(cur_mesh->mNumFaces * 3);
        for (unsigned int i = 0; i < cur_mesh->mNumFaces; i++) {
            const auto &face = cur_mesh->mFaces[i];
            Log::Assert(face.mNumIndices == 3, "mesh '{0}' face indices is {1}", face.mNumIndices);
            newMesh->indices[i * 3] = face.mIndices[0];
            newMesh->indices[i * 3 + 1] = face.mIndices[1];
            newMesh->indices[i * 3 + 2] = face.mIndices[2];
        }

        const auto cur_mat = scene->mMaterials[cur_mesh->mMaterialIndex];
        InitTexture(cur_mat, *newMesh, newModel);

        newModel.meshes.push_back(newMesh);
    }

    void model_loader::InitTexture(const aiMaterial *cur_mat, Mesh &newMesh, const Model &newModel) {
        if (!cur_mat) {
            csugl::Log::Warn("Assimp: no material");
            return;
        }
        auto dir = fs::get_file_path(newModel.path);
        GetTexture(cur_mat, aiTextureType_DIFFUSE, newMesh, dir);
        GetTexture(cur_mat, aiTextureType_SPECULAR, newMesh, dir);
        GetTexture(cur_mat, aiTextureType_AMBIENT, newMesh, dir);
        GetTexture(cur_mat, aiTextureType_NORMALS, newMesh, dir);
    }

    void model_loader::GetTexture(const aiMaterial *cur_mat, unsigned int type,
                                  Mesh &newMesh, const std::string &dir) {
        auto tex_am = SGT_Ref_Str_AM(MTexture);
        aiString path;
        for (unsigned int i = 0; i < cur_mat->GetTextureCount(static_cast<aiTextureType>(type)); i++) {
            if (cur_mat->GetTexture(static_cast<aiTextureType>(type), i, &path) == AI_SUCCESS) {
                std::string tex_file = dir + "/" + path.C_Str();
                auto tex_it = tex_am->get(tex_file);
                if (!tex_it) {
                    auto newTexture = MakeRef<MTexture>();
                    switch (static_cast<aiTextureType>(type)) {
                        case aiTextureType_DIFFUSE:
                            newTexture->type = MTextureType::DIFFUSE;
                            break;
                        case aiTextureType_SPECULAR:
                            newTexture->type = MTextureType::SPECULAR;
                            break;
                        case aiTextureType_AMBIENT:
                            newTexture->type = MTextureType::AMBIENT;
                            break;
                        case aiTextureType_NORMALS:
                            newTexture->type = MTextureType::NORMAL;
                            break;
                        default:
                            break;
                    }
                    auto meta_data = stbi_load(tex_file.c_str(),
                                               &newTexture->width,
                                               &newTexture->height,
                                               &newTexture->channel,
                                               0);
                    if (!meta_data) {
                        csugl::Log::Error("Assimp: stbi: Texture load failed at '{0}'", tex_file);
                        continue;
                    }
                    newTexture->data = Scope<unsigned char, decltype(MTexture::data_deleter)>(
                            meta_data,
                            MTexture::data_deleter);
                    tex_am->add(tex_file, newTexture);
                    newMesh.textures.push_back(newTexture);
                } else {
                    newMesh.textures.push_back(tex_it);
                }
            }

        }
    }

}
