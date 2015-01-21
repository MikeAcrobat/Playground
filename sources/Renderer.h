//
//  Renderer.h
//  3d technics
//
//  Created by Mike on 12/6/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#ifndef ___d_technics__Renderer__
#define ___d_technics__Renderer__

#ifdef DEBUG
    #define GL_DEBUG(x) x; Renderer::check_error(__FILE__,__LINE__);
#else
    #define GL_DEBUG(x) x;
#endif

class Renderer {
public:
    static void check_error(const char *file, int line);
    
    static void init(unsigned int width, unsigned int height);
    static void cleanup();
    
    static void render_frame(Mesh::Ptr mesh, Camera::Ptr camera);
    
    static void full_screen_path();
};

#endif /* defined(___d_technics__Renderer__) */
