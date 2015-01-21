//
//  Utility.h
//  3d technics
//
//  Created by Mike on 12/10/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#ifndef ___d_technics__Utility__
#define ___d_technics__Utility__

#include <string>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Utility {
public:
    static bool load_file(std::vector<char> & buffer, const std::string & filepath);
    static std::string get_resource_folder();
};

#endif /* defined(___d_technics__Utility__) */
