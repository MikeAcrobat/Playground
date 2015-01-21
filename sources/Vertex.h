//
//  Vertex.h
//  3d technics
//
//  Created by Mike on 12/11/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#ifndef ___d_technics__Vertex__
#define ___d_technics__Vertex__

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

//class VertexDelaration {
//    
//    struct Attrib {
//        Shader::Attributes  type;
//        unsigned int        offset;
//        unsigned int        components;
//    };
//    
//    unsigned int size;
//    
//    std::vector<Attrib> attributes;
//
//public:
//    
//    VertexDelaration() : size(0) {
//        
//    }
//    
//    VertexDelaration & add(Shader::Attributes attribute_type, unsigned int components) {
//        Attrib att;
//        att.components  = components;
//        att.type        = attribute_type;
//        att.offset      = size;
//        attributes.push_back(att);
//        
//        size += components * sizeof(float);
//        
//        return *this;
//    }
//    
//    unsigned int components_count(Shader::Attributes attribute_type) {
//        for (auto att : attributes) if (att.type == attribute_type) return att.components;
//        return 0;
//    }
//    
//    unsigned int attribute_offset(Shader::Attributes attribute_type) {
//        for (auto att : attributes) if (att.type == attribute_type) return att.offset;
//        return 0;
//    }
//    
//    unsigned int vertex_size() {
//        return size;
//    }
//};

#endif /* defined(___d_technics__Vertex__) */
