//
//  Mesh.h
//  3d technics
//
//  Created by Mike on 12/7/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#ifndef ___d_technics__Mesh__
#define ___d_technics__Mesh__

#include "Material.h"
#include "Vertex.h"

struct MeshInfo {
    struct SubMeshInfo {
        std::string                 name;
        std::vector<Vertex>         vertexes;
        std::vector<unsigned int>   indexes;
        MaterialInfo                material;
    };
    
    std::string                 name;
    std::vector<SubMeshInfo>    sub_meshes;
};

class Mesh : public std::enable_shared_from_this<Mesh> {
    
    friend class Renderer;
    
public:
    typedef std::shared_ptr<Mesh> Ptr;
    typedef std::function<void (const Mesh::Ptr &)> RenderFunc;
    
    static Mesh::Ptr create(const MeshInfo &);
    static Mesh::Ptr load_from_file(const std::string & base_directory, const std::string & model_name);

    void render(RenderFunc render_func);
    
private:
    void init(const MeshInfo &);
    
    struct SubMeshChunk {
        MeshBuffer::Chunk   buffer_chunk;
        std::string         name;
        Material::Ptr       material;
    };
    
    std::string                 name;
    std::vector<SubMeshChunk>   sub_meshes;
    
    MeshBuffer::Ptr             buffer;
};

#endif /* defined(___d_technics__Mesh__) */
