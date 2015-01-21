//
//  Camera.cpp
//  3d technics
//
//  Created by Mike on 12/11/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#include "Camera.h"

Camera::Ptr Camera::create() {
    Camera::Ptr camera = std::make_shared<Camera>();
    camera->init_perspective();
    return camera;
}

void Camera::init_perspective(float fov, float aspect, float near, float far) {
    camera_fov      = fov;
    camera_aspect   = aspect;
    near_plane      = near;
    far_plane       = far;

    camera_position = glm::vec3(500, 500, 500);
    camera_direction = - glm::vec3(1, 1, 1);
}

glm::mat4 Camera::get_projection() {
    return glm::perspective(camera_fov, camera_aspect, near_plane, far_plane);
}

glm::mat4 Camera::get_view() {
    return glm::lookAt(camera_position, camera_position + camera_direction, glm::vec3(0, 1, 0));
}

void Camera::set_camera_position(const glm::vec3 & position) {
    camera_position = position;
}

void Camera::set_camera_direction(const glm::vec3 & direction) {
    camera_direction = direction;
}