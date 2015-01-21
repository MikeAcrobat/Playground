//
//  Application.h
//  3d technics
//
//  Created by Mike on 12/6/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#ifndef ___d_technics__Application__
#define ___d_technics__Application__

class Application {
    
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void motion_callback(GLFWwindow* window, double x, double y);
    
    static bool application_init(unsigned int width, unsigned int height);
    static void application_cleanup();
    
    static void update_ui();
    
    static void update_application();
public:
    static int run();
    
    static float get_delta_time();
};

#endif /* defined(___d_technics__Application__) */
