//
//  MeshBuffer.h
//  3d technics
//
//  Created by Mike on 12/14/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#ifndef ___d_technics__MeshBuffer__
#define ___d_technics__MeshBuffer__

class MeshBuffer {
public:
    struct Chunk {
        unsigned int index_offset;
        unsigned int index_count;
    };
    
    typedef std::shared_ptr<MeshBuffer> Ptr;
    
    MeshBuffer();
    ~MeshBuffer();
    
    void bind();
    void unbind();
    
    Chunk add_to_buffer(const std::vector<Vertex> & b, std::vector<unsigned int> i);
    
    void update();
private:
    
    std::vector<Vertex> vertexes;
    std::vector<unsigned int> indexes;
    
    GLuint vao, vbo, ibo;
    
    unsigned int index_offset;
    unsigned int vertex_offset;
};

class MeshBufferBinder {
private:
    MeshBuffer::Ptr mesh_buffer;
public:
    MeshBufferBinder(MeshBuffer::Ptr buffer) : mesh_buffer(buffer) {
        if (mesh_buffer) {
            mesh_buffer->bind();
        }
    }
    
    ~MeshBufferBinder() {
        if (mesh_buffer) {
            mesh_buffer->unbind();
        }
    }
};

#endif /* defined(___d_technics__MeshBuffer__) */
