//
//  Material.h
//  3d technics
//
//  Created by Mike on 12/10/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#ifndef ___d_technics__Material__
#define ___d_technics__Material__

struct MaterialInfo {
    std::string diffuse_texture;
    std::string normal_texture;
    std::string specular_texture;
    float       specular_shininess;
};

class Material {
public:
    typedef std::shared_ptr<Material> Ptr;
    
    static Material::Ptr create(const MaterialInfo &);
    
    void bind(Shader::Ptr program);
    
private:
    void init(const MaterialInfo &);
    
    Texture::Ptr diffuse_texture;
    Texture::Ptr normal_texture;
    Texture::Ptr specular_texture;
    
    float        specular_shininess;
};

#endif /* defined(___d_technics__Material__) */
