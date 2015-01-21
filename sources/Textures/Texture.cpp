//
//  Texture.cpp
//  3d technics
//
//  Created by Mike on 12/13/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture2D::Texture2D(unsigned int width, unsigned int height, GLint inner_format, GLenum format, GLenum pixel_type, bool mipmaps, unsigned char * pixels) :
name(0),
inner_format(inner_format),
format(format),
pixel_type(pixel_type),
filter_mag(GL_LINEAR),
filter_min(GL_LINEAR_MIPMAP_LINEAR),
wrap_s(GL_REPEAT),
wrap_t(GL_REPEAT),
width(width),
height(height)
{
    GL_DEBUG(glGenTextures(1, &name));
    GL_DEBUG(glActiveTexture(GL_TEXTURE0));
    GL_DEBUG(glBindTexture(GL_TEXTURE_2D, name));
    
    GL_DEBUG(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    GL_DEBUG(glTexImage2D(GL_TEXTURE_2D, 0, inner_format, width, height, 0, format, pixel_type, pixels));
    
    GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min));
    GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag));

    GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
    GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
    
    if (mipmaps) {
        GL_DEBUG(glGenerateMipmap(GL_TEXTURE_2D));
    }
}

Texture2D::~Texture2D() {
    if (name) {
        GL_DEBUG(glDeleteTextures(1, &name));
    }
}

void Texture2D::bind(unsigned int index) {
    GL_DEBUG(glActiveTexture(GL_TEXTURE0 + index));
    GL_DEBUG(glBindTexture(GL_TEXTURE_2D, name));

    GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min));
    GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag));
    
    GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
    GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
}

void Texture2D::unbind() {
    
}

void Texture2D::set_wrap(GLint s, GLint t) {
    wrap_s = s;
    wrap_t = t;
}

void Texture2D::set_filter(GLint mag, GLint min) {
    filter_mag = mag;
    filter_min = min;
}


/*
 
 */


/*
 
 */
static std::unordered_map<std::string, Texture2D::Ptr> cache;

Texture2D::Ptr TextureCache::load_from_file(const std::string & file) {
    
    {   // check cache
        auto tex_it = cache.find(file);
        if (tex_it != cache.end()) return tex_it->second;
    }
    
    Texture2D::Ptr texture = nullptr;
    
    std::vector<char> buffer;
    if (Utility::load_file(buffer, file)) {
        int x, y, components;
        unsigned char * data = stbi_load_from_memory((unsigned char *)buffer.data(), (int)buffer.size(), &x, &y, &components, 0);
        if (data) {
            GLint inner_format = 0;
            GLenum format = 0;
            if (components == 1) { inner_format = GL_R8;     format = GL_RED;   }
            if (components == 2) { inner_format = GL_RG8;    format = GL_RG;    }
            if (components == 3) { inner_format = GL_RGB8;   format = GL_RGB;   }
            if (components == 4) { inner_format = GL_RGBA8;  format = GL_RGBA;  }
            
            assert(inner_format != 0 && format != 0);
            
            texture = std::make_shared<Texture2D>(x, y, inner_format, format, GL_UNSIGNED_BYTE, true, data);
            
            stbi_image_free(data);
        }
    }
    
    cache[file] = texture;
    return texture;
}
