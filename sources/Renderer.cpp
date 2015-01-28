//
//  Renderer.cpp
//  3d technics
//
//  Created by Mike on 12/6/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#include "Renderer.h"

static FrameBuffer::Ptr render_target;
static FrameBuffer::Ptr g_buffer;
static unsigned int     window_w, window_h;
static MeshBuffer::Ptr  quad;

void Renderer::check_error(const char *file, int line) {
    GLenum err (glGetError());
    
    while(err != GL_NO_ERROR) {
        std::string error;
        
        switch(err) {
            case GL_INVALID_OPERATION:              error = "INVALID_OPERATION";                break;
            case GL_INVALID_ENUM:                   error = "INVALID_ENUM";                     break;
            case GL_INVALID_VALUE:                  error = "INVALID_VALUE";                    break;
            case GL_OUT_OF_MEMORY:                  error = "OUT_OF_MEMORY";                    break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";    break;
        }
        
        printf("GL_%s - %s : %d\n", error.c_str(), file, line);
        err = glGetError();
    }
}

void Renderer::init(unsigned int width, unsigned int height) {
    
    g_buffer = std::make_shared<FrameBuffer>();

    g_buffer->bind_buffer(GL_FRAMEBUFFER);
    g_buffer->attach(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0);
    g_buffer->attach(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT1);
    g_buffer->attach(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT2);
    
    g_buffer->attach(width, height, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
    
    bool g_complete = g_buffer->complete();
    
    render_target = std::make_shared<FrameBuffer>();
    render_target->bind_buffer(GL_FRAMEBUFFER);
    render_target->attach(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0);
    bool r_complete = render_target->complete();
    
    {
        quad = std::make_shared<MeshBuffer>();
        
        std::vector<Vertex> vertexes = {
            {{  1.0,  1.0, 0.0 }, { 1.0, 1.0 }, {  1.0,  0.0, 0.0 }, {  0.0,  1.0, 0.0 }, {  0.0,  0.0, 1.0 }},
            {{ -1.0,  1.0, 0.0 }, { 0.0, 1.0 }, {  1.0,  0.0, 0.0 }, {  0.0,  1.0, 0.0 }, {  0.0,  0.0, 1.0 }},
            {{ -1.0, -1.0, 0.0 }, { 0.0, 0.0 }, {  1.0,  0.0, 0.0 }, {  0.0,  1.0, 0.0 }, {  0.0,  0.0, 1.0 }},
            {{ -1.0, -1.0, 0.0 }, { 0.0, 0.0 }, {  1.0,  0.0, 0.0 }, {  0.0,  1.0, 0.0 }, {  0.0,  0.0, 1.0 }},
            {{  1.0, -1.0, 0.0 }, { 1.0, 0.0 }, {  1.0,  0.0, 0.0 }, {  0.0,  1.0, 0.0 }, {  0.0,  0.0, 1.0 }},
            {{  1.0,  1.0, 0.0 }, { 1.0, 1.0 }, {  1.0,  0.0, 0.0 }, {  0.0,  1.0, 0.0 }, {  0.0,  0.0, 1.0 }}
        };
        
        std::vector<unsigned int> indexes = { 0, 1, 2, 3, 4, 5 };
        
        quad->add_to_buffer(vertexes, indexes);
        quad->update();
    }
    
    window_w = width;   window_h = height;
}

void Renderer::cleanup() {
    g_buffer = nullptr;
}

void Renderer::render_frame(Mesh::Ptr __mesh__, Camera::Ptr camera) {
    
    // GEOMETRY PASS
    {
        g_buffer->bind_buffer(GL_FRAMEBUFFER);
        g_buffer->setup_draw_attachments({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 });
        
        GL_DEBUG(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GL_DEBUG(glEnable(GL_DEPTH_TEST));
        
        Shader::Ptr shader = ShaderPool::get_shader(ShaderPool::Type::Geometry);
        
        const auto & render_sub_mesh = [] (const MeshBuffer::Chunk & chunk) {
            GL_DEBUG(glDrawElements(GL_TRIANGLES, (GLsizei)chunk.index_count, GL_UNSIGNED_INT, (void*)(chunk.index_offset * sizeof(unsigned int))));
        };
        
        const Mesh::RenderFunc & mesh_renderer = [=] (Mesh::Ptr mesh) {
            MeshBufferBinder binding(mesh->buffer);
            
            const glm::mat4 & projection = camera->get_projection();
            const glm::mat4 & view       = camera->get_view();
            const glm::mat4 & model      = glm::scale(glm::vec3(.1, .1, .1));
            
            shader->set_value("world",              model);
            shader->set_value("view_projection",    projection * view);
            shader->set_value("normal_matrix",      glm::mat3(glm::inverse(model)));
            
            for (const auto & sub_mesh : mesh->sub_meshes) {
                sub_mesh.material->bind(shader);
                render_sub_mesh(sub_mesh.buffer_chunk);
            }
        };

        shader->bind();
        __mesh__->render(mesh_renderer);

        g_buffer->unbind();
        
        /*******/
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        render_target->bind_buffer(GL_FRAMEBUFFER);
        render_target->setup_draw_attachments( { GL_COLOR_ATTACHMENT0 } );
        
        glDepthMask(GL_FALSE);
        GL_DEBUG(glClear(GL_COLOR_BUFFER_BIT));
        GL_DEBUG(glDisable(GL_DEPTH_TEST));
        
        Shader::Ptr test_shader = ShaderPool::get_shader(ShaderPool::Type::Test);
        test_shader->bind();

        if (g_buffer->get_texture(GL_COLOR_ATTACHMENT0)) {
            test_shader->bind_sampler("diffuse_texture", 0);
            g_buffer->get_texture(GL_COLOR_ATTACHMENT0)->bind(0);
        }
        
        if (g_buffer->get_texture(GL_COLOR_ATTACHMENT1)) {
            test_shader->bind_sampler("normal_texture", 1);
            g_buffer->get_texture(GL_COLOR_ATTACHMENT1)->bind(1);
        }
        
        //TextureCache::load_from_file("crytek-sponza/zero_normal.tga")->bind(0);

        Renderer::full_screen_path();
        
        render_target->unbind();
    }
    GLint half_w = (GLint)(window_w / 2.0f);
    GLint half_h = (GLint)(window_h / 2.0f);

    {
        g_buffer->bind_buffer(GL_READ_FRAMEBUFFER);
        g_buffer->display_color_attachment(GL_COLOR_ATTACHMENT0, 0,      half_h, half_w,     window_h);
        g_buffer->display_color_attachment(GL_COLOR_ATTACHMENT1, half_w, half_h, window_w,   window_h);
        g_buffer->display_color_attachment(GL_COLOR_ATTACHMENT2, 0,      0,      half_w,     half_h);
        
        g_buffer->unbind();
    }
    {
        render_target->bind_buffer(GL_READ_FRAMEBUFFER);
        render_target->display_color_attachment(GL_COLOR_ATTACHMENT0, half_w, 0, window_w, half_h);
        render_target->unbind();
    }
}

void Renderer::full_screen_path() {
    quad->bind();
    GL_DEBUG(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    quad->unbind();
}