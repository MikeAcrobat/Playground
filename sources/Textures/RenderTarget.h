//
//  RenderTarget.h
//  3d technics
//
//  Created by Mike on 1/12/15.
//  Copyright (c) 2015 Mike. All rights reserved.
//

#ifndef ___d_technics__RenderTarget__
#define ___d_technics__RenderTarget__

class FrameBuffer {
    GLuint fbo;
    
    std::map<unsigned int, Texture2D::Ptr> attachments;
    
public:
    typedef std::shared_ptr<FrameBuffer> Ptr;

    FrameBuffer(void);
    ~FrameBuffer(void);
  
    void attach(unsigned int width, unsigned int height, GLint inner_format, GLenum format, GLenum pixel_type, GLenum target);
    bool complete();
    
    Texture2D::Ptr get_texture(GLenum target);
    
    void bind_buffer(GLenum target, std::vector<GLenum> attachments = std::vector<GLenum>());
    void unbind();
    
    void display_color_attachment(GLenum attachment, float x0, float y0, float x1, float y1);
};

//class RenderTarget {
//
//    GLuint fbo, texture;
//    
//    GLint inner_format;
//    GLenum format;
//    GLenum pixel_type;
//    
//    GLint wrap_s, wrap_t;
//    GLint filter_mag, filter_min;
//    
//    unsigned int width, height;
//
//public:
//    typedef std::shared_ptr<RenderTarget> Ptr;
//    
//    RenderTarget(unsigned int width, unsigned int height, GLint inner_format, GLenum format, GLenum pixel_type, bool mipmaps);
//    virtual ~RenderTarget(void);
//    
//    virtual void bind(unsigned int channel) override;
//    virtual void unbind() override;
//
//    virtual void set_wrap(GLint s, GLint t) override;
//    virtual void set_filter(GLint mag, GLint min) override;
//    
//    //    void bind_for_writing();
//    //    void bind_for_reading();
//};


#endif /* defined(___d_technics__RenderTarget__) */
