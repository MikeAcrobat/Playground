//
//  Material.cpp
//  3d technics
//
//  Created by Mike on 12/10/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#include "Material.h"

Material::Ptr Material::create(const MaterialInfo & data) {
    Material::Ptr material = std::make_shared<Material>();
    material->init(data);
    return material;
}

void Material::init(const MaterialInfo & data) {
    diffuse_texture      = TextureCache::load_from_file(data.diffuse_texture);
    normal_texture       = TextureCache::load_from_file(data.normal_texture);
    specular_texture     = TextureCache::load_from_file(data.specular_texture);
    
    if (normal_texture == nullptr) {
        normal_texture   = TextureCache::load_from_file("crytek-sponza/zero_normal.tga");
    }
    
    if (specular_texture == nullptr) {
        specular_texture = TextureCache::load_from_file("crytek-sponza/zero_specular.tga");
    }
    
    specular_shininess   = data.specular_shininess;
}

void Material::bind(Shader::Ptr program) {
    if (program) {
        program->bind();
        
        const int diffuse_texture_channel   = 0;
        const int normal_texture_channel    = 1;
        const int specular_texture_channel  = 2;
        
        if (diffuse_texture) {
            diffuse_texture->bind(diffuse_texture_channel);
            program->bind_sampler("diffuse_texture", diffuse_texture_channel);
        }
        
        if (normal_texture) {
            normal_texture->bind(normal_texture_channel);
            program->bind_sampler("normal_texture", normal_texture_channel);
        }

        if (specular_texture) {
            specular_texture->bind(specular_texture_channel);
            program->bind_sampler("specular_texture", specular_texture_channel);
        }
        
        program->set_value("specular_shininess", specular_shininess);
    }
}