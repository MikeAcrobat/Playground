//
//  Mesh.cpp
//  3d technics
//
//  Created by Mike on 12/7/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#include "tiny_obj_loader.h"

Mesh::Ptr Mesh::create(const MeshInfo & mesh_data) {
    Mesh::Ptr mesh = std::make_shared<Mesh>();
    mesh->init(mesh_data);
    return mesh;
}

Mesh::Ptr Mesh::load_from_file(const std::string & base_directory, const std::string & model_name) {

    std::string model_path = Utility::get_resource_folder() + base_directory + "/" + model_name;
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(model_path,
                                             aiProcess_CalcTangentSpace       |
                                             aiProcess_Triangulate            |
                                             aiProcess_JoinIdenticalVertices  |
                                             aiProcess_FlipUVs                |
                                             aiProcess_GenNormals             |
                                             aiProcess_SortByPType);
    
    MeshInfo data;
    data.name = model_name;
    data.sub_meshes.resize(scene->mNumMeshes);

    auto vector_3d_converter = [] (const aiVector3D & vector) { return glm::vec3(vector.x, vector.y, vector.z); };
    auto vector_2d_converter = [] (const aiVector3D & vector) { return glm::vec2(vector.x, vector.y); };
    
    auto extract_texture = [base_directory] (aiMaterial* material, aiTextureType type, std::string & path) {
        aiString texture_path;
        aiTextureMapMode mapmode;
        if (material->GetTexture(type, 0, &texture_path, NULL, NULL, NULL, NULL, &mapmode) == AI_SUCCESS) {
            path = base_directory + "/" + texture_path.C_Str();
        }
    };
    
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh * mesh = scene->mMeshes[i];
        MeshInfo::SubMeshInfo & sub_mesh = data.sub_meshes[i];
        
        sub_mesh.name = mesh->mName.C_Str();
        sub_mesh.vertexes.reserve(mesh->mNumVertices);
        
        for (auto v = 0; v < mesh->mNumVertices; v++) {
            Vertex vertex;
            
            vertex.pos          = vector_3d_converter(mesh->mVertices[v]);
            vertex.uv           = mesh->HasTextureCoords(0)         ? vector_2d_converter(mesh->mTextureCoords[0][v])   : glm::vec2(0.0f, 0.0f);
            vertex.normal       = mesh->HasNormals()                ? vector_3d_converter(mesh->mNormals[v])            : glm::vec3(1.0f, 1.0f, 1.0f);
            vertex.tangent      = mesh->HasTangentsAndBitangents()  ? vector_3d_converter(mesh->mTangents[v])           : glm::vec3(1.0f, 1.0f, 1.0f);
            vertex.bitangent    = mesh->HasTangentsAndBitangents()  ? vector_3d_converter(mesh->mBitangents[v])         : glm::vec3(1.0f, 1.0f, 1.0f);
            
            sub_mesh.vertexes.push_back(vertex);
        }
        
        sub_mesh.indexes.reserve(mesh->mNumFaces * 3);
        
        for (auto f = 0; f < mesh->mNumFaces; f++) {
            const aiFace& face = mesh->mFaces[f];
            sub_mesh.indexes.push_back(face.mIndices[0]);
            sub_mesh.indexes.push_back(face.mIndices[1]);
            sub_mesh.indexes.push_back(face.mIndices[2]);
        }
        
        unsigned int material_id = mesh->mMaterialIndex;
        aiMaterial* material = scene->mMaterials[material_id];
        
        extract_texture(material, aiTextureType_DIFFUSE,    sub_mesh.material.diffuse_texture);
        extract_texture(material, aiTextureType_HEIGHT,     sub_mesh.material.normal_texture);
        extract_texture(material, aiTextureType_SPECULAR,   sub_mesh.material.specular_texture);
        
        float shininess = 0.f;
        if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess) && shininess >= 0.0f && shininess <= 128.0f) {
            sub_mesh.material.specular_shininess = shininess;
        }
    }
    
    return create(data);
}

void Mesh::render(RenderFunc render_func) {
    render_func(shared_from_this());
}

void Mesh::init(const MeshInfo & data) {
    name = data.name;
    sub_meshes.reserve(data.sub_meshes.size());

    buffer = std::make_shared<MeshBuffer>();
    for (const auto & sub_mesh_data : data.sub_meshes) {
        SubMeshChunk sub_mesh_chunk;
        sub_mesh_chunk.material     = Material::create(sub_mesh_data.material);
        sub_mesh_chunk.name         = sub_mesh_data.name;
        sub_mesh_chunk.buffer_chunk = buffer->add_to_buffer(sub_mesh_data.vertexes, sub_mesh_data.indexes);
        sub_meshes.push_back(sub_mesh_chunk);
    }
    buffer->update();
}