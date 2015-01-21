//
//  Application.cpp
//  3d technics
//
//  Created by Mike on 12/6/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#include "Application.h"

#include "Mesh.h"

//#include <Awesomium/WebCore.h>
//#include <Awesomium/BitmapSurface.h>

class CameraController {
    Camera::Ptr camera;
    glm::vec3 position, direction;
    glm::vec2 cursor_position;
    
    float move_speed, time;
    
    void update_camera() {
        if (camera) {
            camera->set_camera_direction(direction);
            camera->set_camera_position(position);
        }
    }
public:
    CameraController(Camera::Ptr cam) {
        camera = cam;
        position = glm::vec3(0, 100, 0);
        direction = glm::vec3(0, 0, 1);
        move_speed = 300.f;
        update_camera();
    }
    
    void move_forward() {
        position += glm::normalize(direction) * move_speed * Application::get_delta_time();
        update_camera();
    }
    
    void move_backward() {
        position -= glm::normalize(direction) * move_speed * Application::get_delta_time();
        update_camera();
    }
    
    void strife_left() {
        glm::vec3 axis = glm::cross(direction, glm::vec3(0, 1, 0));
        position -= glm::normalize(axis) * move_speed * Application::get_delta_time();
        update_camera();
    }
    
    void strife_right() {
        glm::vec3 axis = glm::cross(direction, glm::vec3(0, 1, 0));
        position += glm::normalize(axis) * move_speed * Application::get_delta_time();
        update_camera();
    }
    
    void change_direction(const glm::vec2 & mouse) {
        glm::vec2 diff = cursor_position - mouse;
        glm::vec3 camera_up(0, 1, 0);
        glm::vec3 axis = glm::normalize(glm::cross(direction, camera_up));
        glm::quat vertical_rotation = glm::angleAxis(glm::radians(diff.y), axis);
        glm::quat horizont_rotation = glm::angleAxis(glm::radians(diff.x), camera_up);
        glm::quat conjunction = vertical_rotation * horizont_rotation; // glm::normalize(glm::cross(horizont_rotation, vertical_rotation));
        direction = glm::rotate(conjunction, direction);
        cursor_position = mouse;
        update_camera();
    }
};

static CameraController controller(nullptr);

void Application::error_callback(int error, const char* description) {
    printf("%s", description);
}

static struct {
    GLFWwindow * window;
//    Awesomium::WebCore * web_core;
//    Awesomium::WebView * view;
} app;

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void Application::motion_callback(GLFWwindow* window, double x, double y) {
//    app.view->InjectMouseMove(x, y);
    
    controller.change_direction(glm::vec2(x, y));
}

bool Application::application_init(unsigned int width, unsigned int height) {
    
    glfwSetErrorCallback(&Application::error_callback);
    
    if (!glfwInit()) {
        return EXIT_FAILURE;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    app.window = glfwCreateWindow(width, height, "Bla-bla-bla", NULL, NULL);
    
    if (app.window == nullptr) {
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(app.window);
    
    glfwSwapInterval(1);
    
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return false;
    } else {
        glGetError();   // skip first invalid enum error
    }
    
    glfwSetKeyCallback(app.window, &Application::key_callback);
    glfwSetCursorPosCallback(app.window, &Application::motion_callback);
    
    Renderer::init(width, height);
    
//    app.web_core = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
//    app.view = app.web_core->CreateWebView(width, height);
    //app.view->LoadURL()
    
    return true;
}

void Application::application_cleanup() {
//    app.view->Destroy();
//    Awesomium::WebCore::Shutdown();
    
    Renderer::cleanup();
    glfwDestroyWindow(app.window);
    glfwTerminate();
}

void Application::update_ui() {
//    if (!app.view->IsLoading()) {
//        Awesomium::BitmapSurface * surface = (Awesomium::BitmapSurface *)app.view->surface();
//        if (surface) {
//            
//        }
//    }
//
//    app.web_core->Update();
}

int Application::run() {

    if (!Application::application_init(1024, 768)) return EXIT_FAILURE;
    
    Mesh::Ptr mesh = Mesh::load_from_file("crytek-sponza", "sponza.obj");
    
    Camera::Ptr camera = Camera::create();
    
    camera->init_perspective(45, 4.f / 3.f, 1.f, 1000.f);
    
    controller = CameraController(camera);
    
    auto update_camera_controller = [] () {
        auto check_function = [] (int key, std::function<void (void)> callback) {
            auto status = glfwGetKey(app.window, key);
            if (status == GLFW_PRESS || status == GLFW_REPEAT) callback();
        };
        
        check_function(GLFW_KEY_W, std::bind(&CameraController::move_forward,   &controller));
        check_function(GLFW_KEY_S, std::bind(&CameraController::move_backward,  &controller));
        check_function(GLFW_KEY_A, std::bind(&CameraController::strife_left,    &controller));
        check_function(GLFW_KEY_D, std::bind(&CameraController::strife_right,   &controller));
    };
    
    static int current_changing_parameter = 0;
    
    glfwSetScrollCallback(app.window, [] (GLFWwindow*, double, double dy) {
    });
    
    auto update_input = [] () {
        auto status = glfwGetKey(app.window, GLFW_KEY_1);
        if (status == GLFW_PRESS && status != GLFW_REPEAT) current_changing_parameter = (current_changing_parameter + 1) % 3;
    };
    
    while (!glfwWindowShouldClose(app.window)) {
        update_application();
        
        update_camera_controller();
        
        update_input();
        
        Renderer::render_frame(mesh, camera);
        
        glfwSwapBuffers(app.window);
        glfwPollEvents();
    }
    
    controller = CameraController(nullptr);
    
    application_cleanup();
    
    return EXIT_SUCCESS;
}

static float delta_time = 0.f;

void Application::update_application() {
    static float current_frame_time = glfwGetTime(), last_frame_time = current_frame_time;
    
    current_frame_time = glfwGetTime();
    delta_time = current_frame_time - last_frame_time;
    last_frame_time = current_frame_time;
}

float Application::get_delta_time() {
    return delta_time;
}