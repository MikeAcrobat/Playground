//
//  TextureProtocol.h
//  3d technics
//
//  Created by Mike on 1/20/15.
//  Copyright (c) 2015 Mike. All rights reserved.
//

#ifndef ___d_technics__TextureProtocol__
#define ___d_technics__TextureProtocol__

class Texture {
public:
    typedef std::shared_ptr<Texture> Ptr;
    
    virtual ~Texture(void) {}
    
    virtual void bind(unsigned int channel) = 0;
    virtual void unbind() = 0;
    virtual void set_wrap(GLint s, GLint t) = 0;
    virtual void set_filter(GLint mag, GLint min) = 0;
};

#endif /* defined(___d_technics__TextureProtocol__) */
