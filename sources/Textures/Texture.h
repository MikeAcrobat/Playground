//
//  Texture.h
//  3d technics
//
//  Created by Mike on 12/13/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#ifndef ___d_technics__Texture__
#define ___d_technics__Texture__

#include "TextureProtocol.h"

class Texture2D : public Texture {
    
    GLuint name;
    
    GLint inner_format;
    GLenum format;
    GLenum pixel_type;
    
    GLint wrap_s, wrap_t;
    GLint filter_mag, filter_min;
    
    unsigned int width, height;
    
public:
    typedef std::shared_ptr<Texture2D> Ptr;
    
    Texture2D(void) = delete;
    Texture2D(unsigned int width, unsigned int height, GLint inner_format, GLenum format, GLenum pixel_type, bool mipmaps, unsigned char * pixels = nullptr);
    
    virtual ~Texture2D(void);
    
    virtual void bind(unsigned int channel) override;
    virtual void unbind() override;

    virtual void set_wrap(GLint s, GLint t) override;
    virtual void set_filter(GLint mag, GLint min) override;
    
    GLuint texture_name() { return name; }
    
    glm::vec2 texture_size() { return glm::vec2(width, height); }
};

class TextureCache {
public:
    static Texture2D::Ptr load_from_file(const std::string & file);
};

#endif /* defined(___d_technics__Texture__) */
