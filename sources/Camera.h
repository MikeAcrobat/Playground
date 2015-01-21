//
//  Camera.h
//  3d technics
//
//  Created by Mike on 12/11/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#ifndef ___d_technics__Camera__
#define ___d_technics__Camera__

class Camera {
    
    float camera_fov, camera_aspect, near_plane, far_plane;
    
    glm::vec3 camera_position;
    glm::vec3 camera_direction;
    
public:
    typedef std::shared_ptr<Camera> Ptr;
    
    static Camera::Ptr create();
    
    void init_perspective(float fov = 45.f, float aspect = 4.f / 3.f, float near = 1.f, float far = 10000.f);
    
    glm::mat4 get_projection();
    glm::mat4 get_view();
    
    glm::vec3 view_position() { return camera_position; }
    glm::vec3 view_direction() { return camera_direction; }
    
    float far_clipping_plane() { return far_plane; }
    float near_clipping_plane() { return near_plane; }
    
    float half_fovy() { return camera_fov; }
    
    void set_camera_position(const glm::vec3 & position);
    void set_camera_direction(const glm::vec3 & direction);
};

#endif /* defined(___d_technics__Camera__) */
