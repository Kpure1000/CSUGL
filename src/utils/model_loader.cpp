//
// Created by ktsq on 2021/8/22.
//
#include <core/core.h>
#include "model_loader.h"
#include "../core/file_system.h"
#include "asset_manager.h"
#include "singleton.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <memory>

#include <glog/logging.h>

namespace csugl::ml {

    std::function<void(unsigned char *)> MTexture::data_deleter = [](unsigned char *img_data) {
        stbi_image_free(img_data);
    };

    bool model_loader::pre_load(const std::string &fileName, const std::string &modelName) {
        auto realModelName = modelName.empty() ? fs::get_file_name(fileName) : modelName;
        if (model_names.find(modelName) != model_names.end()) {
            LOG(WARNING) << "Model '" << modelName << "' existed at '" << model_names[modelName] << "'";
            return true;
        }
        auto models_am = SGT_Ref_Str_AM(MModel);
        auto oldModel = models_am->get(fileName);
        // model exist
        if (oldModel) {
            LOG(WARNING) << "Model existed at '" << fileName << "', add model name '" << realModelName << "'";
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
            LOG(ERROR) << "Assimp: file '" << fileName << "' error info: " << importer.GetErrorString();
            return false;
        }

        if (!scene->HasMeshes()) {
            LOG(ERROR) << "Assimp: there is no mesh in " << fileName;
            return false;
        }

        // Init mesh
        auto newModel = MakeRef<MModel>();
        newModel->path = fileName;

        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            const auto cur_mesh = scene->mMeshes[i];
            InitMesh(cur_mesh, scene, *newModel);
        }

        if (newModel->meshes.empty()) {
            LOG(ERROR) << "Assimp: Model '" << realModelName << "' at '" << fileName << "' load failed with empty mesh";
            return false;
        }
        models_am->add(fileName, newModel);
        model_names.try_emplace(realModelName, fileName);
        return true;
    }

    Ref<MModel> model_loader::load(const std::string &modelName) {
        if (model_names.find(modelName) != model_names.end()) {
            return SGT_Ref_Str_AM(MModel)->get(model_names[modelName]);
        }
        return nullptr;
    }

    void model_loader::InitMesh(const aiMesh *cur_mesh, const aiScene *scene, MModel &newModel) {
        auto newMesh = MakeRef<MMesh>();
        if (!cur_mesh->HasPositions() || !cur_mesh->HasNormals()) {
            LOG(ERROR) << "Assimp: no vertices or normal info in mesh '" << std::string(cur_mesh->mName.data, cur_mesh->mName.length) << "'";
            return;
        }
        newMesh->name = std::string(cur_mesh->mName.data, cur_mesh->mName.length);
        newMesh->vertex_num = cur_mesh->mNumVertices;
        newMesh->vertices = MakeScope<float[]>(cur_mesh->mNumVertices * 3);
        newMesh->normals = MakeScope<float []>(cur_mesh->mNumVertices * 3);
        newMesh->tangents = MakeScope<float[]>(cur_mesh->mNumVertices * 3);
        newMesh->biTangents = MakeScope<float[]>(cur_mesh->mNumVertices * 3);
        newMesh->uvs = MakeScope<float[]>(cur_mesh->mNumVertices * 2);

#define GET_POS_NOR_TAN_BTAN(i) newMesh->vertices[3 * i]  = cur_mesh->mVertices[i].x; \
                                newMesh->vertices[3 * i + 1] = cur_mesh->mVertices[i].y; \
                                newMesh->vertices[3 * i + 2] = cur_mesh->mVertices[i].z; \
                                newMesh->normals[3 * i] = cur_mesh->mNormals[i].x;\
                                newMesh->normals[3 * i + 1] = cur_mesh->mNormals[i].y;\
                                newMesh->normals[3 * i + 2] = cur_mesh->mNormals[i].z; \
                                newMesh->tangents[3 * i] = cur_mesh->mTangents[i].x;\
                                newMesh->tangents[3 * i + 1] = cur_mesh->mTangents[i].y;\
                                newMesh->tangents[3 * i + 2] = cur_mesh->mTangents[i].z; \
                                newMesh->biTangents[3 * i] = cur_mesh->mBitangents[i].x;\
                                newMesh->biTangents[3 * i + 1] = cur_mesh->mBitangents[i].y;\
                                newMesh->biTangents[3 * i + 2] = cur_mesh->mBitangents[i].z;

        if (cur_mesh->HasTextureCoords(0)) {
            for (unsigned int i = 0; i < cur_mesh->mNumVertices; i++) {
                GET_POS_NOR_TAN_BTAN(i)
                newMesh->uvs[2 * i] = cur_mesh->mTextureCoords[0][i].x;
                newMesh->uvs[2 * i + 1] = cur_mesh->mTextureCoords[0][i].y;
            }
        } else {
            LOG(WARNING) << "Assimp: no vertices or normal info in mesh '" << cur_mesh->mName.C_Str() << "'";
            for (unsigned int i = 0; i < cur_mesh->mNumVertices; i++) {
                GET_POS_NOR_TAN_BTAN(i)
                newMesh->uvs[2 * i] = 0.0f;
                newMesh->uvs[2 * i + 1] = 0.0f;
            }
        }
#undef GET_POS_NOR_TAN_BTAN

        // load face indices
        newMesh->indices = MakeScope<unsigned int[]>(cur_mesh->mNumFaces * 3);
        newMesh->indices_num = cur_mesh->mNumFaces;
        for (unsigned int i = 0; i < cur_mesh->mNumFaces; i++) {
            const auto &face = cur_mesh->mFaces[i];
            CHECK(face.mNumIndices == 3) << "mesh '" << newMesh->name << "' face indices is " << face.mNumIndices;
            newMesh->indices[i * 3] = face.mIndices[0];
            newMesh->indices[i * 3 + 1] = face.mIndices[1];
            newMesh->indices[i * 3 + 2] = face.mIndices[2];
        }

        const auto cur_mat = scene->mMaterials[cur_mesh->mMaterialIndex];
        InitTexture(cur_mat, *newMesh, newModel);

        newModel.meshes.push_back(newMesh);
    }

    void model_loader::InitTexture(const aiMaterial *cur_mat, MMesh &newMesh, const MModel &newModel) {
        if (!cur_mat) {
            LOG(WARNING) << "Assimp: no material";
            return;
        }
        auto dir = fs::get_file_path(newModel.path);
        GetTexture(cur_mat, aiTextureType_DIFFUSE, newMesh, dir);
        GetTexture(cur_mat, aiTextureType_SPECULAR, newMesh, dir);
        GetTexture(cur_mat, aiTextureType_AMBIENT, newMesh, dir);
        GetTexture(cur_mat, aiTextureType_NORMALS, newMesh, dir);
    }

    void model_loader::GetTexture(const aiMaterial *cur_mat, unsigned int type,
                                  MMesh &newMesh, const std::string &dir) {
        auto tex_am = SGT_Ref_Str_AM(MTexture);
        aiString path;
        // new MMaterial
        if(!newMesh.material)
            newMesh.material = MakeRef<MMaterial>();

        aiColor3D keyColor;
        cur_mat->Get(AI_MATKEY_COLOR_AMBIENT, keyColor);
        newMesh.material->Ka[0]=keyColor.r;
        newMesh.material->Ka[1]=keyColor.g;
        newMesh.material->Ka[2]=keyColor.b;
        cur_mat->Get(AI_MATKEY_COLOR_DIFFUSE, keyColor);
        newMesh.material->Kd[0]=keyColor.r;
        newMesh.material->Kd[1]=keyColor.g;
        newMesh.material->Kd[2]=keyColor.b;
        cur_mat->Get(AI_MATKEY_COLOR_SPECULAR, keyColor);
        newMesh.material->Ks[0]=keyColor.r;
        newMesh.material->Ks[1]=keyColor.g;
        newMesh.material->Ks[2]=keyColor.b;

        for (unsigned int i = 0; i < cur_mat->GetTextureCount(static_cast<aiTextureType>(type)); i++) {
            if (cur_mat->GetTexture(static_cast<aiTextureType>(type), i, &path) == AI_SUCCESS) {
                std::string tex_file = dir + "/" + path.C_Str();
                auto tex_it = tex_am->get(tex_file);
                if (!tex_it) {
                    auto newTexture = MakeRef<MTexture>();
                    auto meta_data = stbi_load(tex_file.c_str(),
                                               &newTexture->width,
                                               &newTexture->height,
                                               &newTexture->channel,
                                               0);
                    if (!meta_data) {
                        LOG(ERROR) << "Assimp: stbi: Texture load failed at '" << tex_file << "'";
                        continue;
                    }
                    newTexture->data = Scope<unsigned char, decltype(MTexture::data_deleter)>(
                            meta_data,
                            MTexture::data_deleter);
                    tex_am->add(tex_file, newTexture);
                    switch (static_cast<aiTextureType>(type)) {
                        case aiTextureType_DIFFUSE:
                            newTexture->type = MTextureType::DIFFUSE;
                            newMesh.material->map_Kd = newTexture;
                            break;
                        case aiTextureType_SPECULAR:
                            newTexture->type = MTextureType::SPECULAR;
                            newMesh.material->map_Ks = newTexture;
                            break;
                        case aiTextureType_AMBIENT:
                            newTexture->type = MTextureType::AMBIENT;
                            newMesh.material->map_Ka = newTexture;
                            break;
                        case aiTextureType_NORMALS:
                            newTexture->type = MTextureType::NORMAL;
                            break;
                        default:
                            break;
                    }
                } else {
                    switch (tex_it->type) {
                        case MTextureType::DIFFUSE:
                            newMesh.material->map_Kd = tex_it;
                            break;
                        case MTextureType::SPECULAR:
                            newMesh.material->map_Ks = tex_it;
                            break;
                        case MTextureType::AMBIENT:
                            newMesh.material->map_Ka = tex_it;
                            break;
                        case MTextureType::NORMAL:
                            break;
                        default:
                            break;
                    }
                }
            }

        }
    }

}
