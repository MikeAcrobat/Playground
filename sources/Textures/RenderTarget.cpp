//
//  RenderTarget.cpp
//  3d technics
//
//  Created by Mike on 1/12/15.
//  Copyright (c) 2015 Mike. All rights reserved.
//

#include "RenderTarget.h"

FrameBuffer::FrameBuffer(void) : fbo(0) {
    GL_DEBUG(glGenFramebuffers(1, &fbo));
    GL_DEBUG(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
}

FrameBuffer::~FrameBuffer(void) {
    if (fbo) {
        GL_DEBUG(glDeleteFramebuffers(1, &fbo));
    }
}

void FrameBuffer::attach(unsigned int width, unsigned int height, GLint inner_format, GLenum format, GLenum pixel_type, GLenum target) {
    Texture2D::Ptr texture = std::make_shared<Texture2D>(width, height, inner_format, format, pixel_type, false, nullptr);
    GL_DEBUG(glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, texture->texture_name(), 0));
    attachments[target] = texture;
}

bool FrameBuffer::complete() {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", status);
        return false;
    }

    return true;
}

Texture2D::Ptr FrameBuffer::get_texture(GLenum target) {
    auto attached_texture = attachments.find(target);
    return attached_texture != attachments.end() ? attached_texture->second : nullptr;
}

void FrameBuffer::bind_buffer(GLenum target) {
    GL_DEBUG(glBindFramebuffer(target, fbo));
}

void FrameBuffer::setup_draw_attachments(std::vector<GLenum> attachments) {
    if (attachments.size() > 0) {
        GL_DEBUG(glDrawBuffers((GLsizei)attachments.size(), attachments.data()));
    }
}

void FrameBuffer::setup_draw_attachment(GLenum attachment) {
    GL_DEBUG(glDrawBuffer(attachment));
}

void FrameBuffer::setup_read_attachment(GLenum attachment) {
    GL_DEBUG(glReadBuffer(attachment));
}

void FrameBuffer::unbind() {
    GL_DEBUG(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::display_color_attachment(GLenum attachment, float x0, float y0, float x1, float y1) {
    Texture2D::Ptr texture = get_texture(attachment);
    if (texture) {
        GL_DEBUG(glReadBuffer(attachment));
        glm::vec2 size = texture->texture_size();
        GL_DEBUG(glBlitFramebuffer(0, 0, size.x, size.y, x0, y0, x1, y1, GL_COLOR_BUFFER_BIT, GL_LINEAR));
    }
}