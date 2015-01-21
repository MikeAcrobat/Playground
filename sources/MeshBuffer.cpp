//
//  MeshBuffer.cpp
//  3d technics
//
//  Created by Mike on 12/14/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#include "MeshBuffer.h"

void MeshBuffer::bind() {
    GL_DEBUG(glBindVertexArray(vao));
}

void MeshBuffer::unbind() {
    GL_DEBUG(glBindVertexArray(0));
}


MeshBuffer::MeshBuffer() {
    vao = index_offset = vertex_offset = 0;
    
    GL_DEBUG(glGenVertexArrays(1, &vao));
    
    GL_DEBUG(glBindVertexArray(vao));
    
    GL_DEBUG(glGenBuffers(1, &vbo));
    GL_DEBUG(glGenBuffers(1, &ibo));
    
    GL_DEBUG(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_DEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    
    GL_DEBUG(glVertexAttribPointer(Shader::ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos)));
	GL_DEBUG(glEnableVertexAttribArray(Shader::ATTRIBUTE_POSITION));
	
	GL_DEBUG(glVertexAttribPointer(Shader::ATTRIBUTE_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv)));
	GL_DEBUG(glEnableVertexAttribArray(Shader::ATTRIBUTE_TEXTURE_COORD));
	
	GL_DEBUG(glVertexAttribPointer(Shader::ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal)));
	GL_DEBUG(glEnableVertexAttribArray(Shader::ATTRIBUTE_NORMAL));
    
    GL_DEBUG(glVertexAttribPointer(Shader::ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent)));
	GL_DEBUG(glEnableVertexAttribArray(Shader::ATTRIBUTE_TANGENT));
    
    GL_DEBUG(glVertexAttribPointer(Shader::ATTRIBUTE_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bitangent)));
	GL_DEBUG(glEnableVertexAttribArray(Shader::ATTRIBUTE_BITANGENT));
    
    GL_DEBUG(glBindVertexArray(0));
    GL_DEBUG(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_DEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

MeshBuffer::~MeshBuffer() {
    GL_DEBUG(glBindVertexArray(0));

    if (vao) {
        GL_DEBUG(glDeleteVertexArrays(1, &vao));
    }
    
    GL_DEBUG(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_DEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    if (vbo) {
        GL_DEBUG(glDeleteBuffers(1, &vbo));
    }

    if (ibo) {
        GL_DEBUG(glDeleteBuffers(1, &ibo));
    }
}

MeshBuffer::Chunk MeshBuffer::add_to_buffer(const std::vector<Vertex> & v, std::vector<unsigned int> i) {
    MeshBuffer::Chunk chunk;
    
    chunk.index_offset = (unsigned int)indexes.size();
    chunk.index_count = (unsigned int)i.size();
    
    for (auto & index : i) index += vertexes.size();
    
    vertexes.insert(vertexes.end(), v.begin(), v.end());
    indexes.insert(indexes.end(), i.begin(), i.end());
    
    return chunk;
}

void MeshBuffer::update() {
    GL_DEBUG(glBindVertexArray(vao));
    
    GL_DEBUG(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_DEBUG(glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(Vertex), vertexes.data(), GL_STATIC_DRAW));
    GL_DEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GL_DEBUG(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(unsigned int), indexes.data(), GL_STATIC_DRAW));
    
    GL_DEBUG(glBindVertexArray(0));
}