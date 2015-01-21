//
//  Utility.mm
//  3d technics
//
//  Created by Mike on 12/10/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "Utility.h"

bool Utility::load_file(std::vector<char> & buffer, const std::string & filepath) {
    if (filepath.empty()) return false;
    std::string fixed_path = get_resource_folder() + filepath;
    std::ifstream file(fixed_path, std::ios::binary);
	if (file.is_open()) {
		file.unsetf(std::ios::skipws);
		file.seekg(0, std::ios::end);
		size_t length = (size_t)file.tellg();
		file.seekg(0, std::ios::beg);
		
		buffer.resize(length + 1);
		file.read(buffer.data(), length);
		buffer[length] = 0;
		return true;
	} else {
		printf("Failed to open file : %s \n", fixed_path.c_str());
		return false;
	}
}

std::string Utility::get_resource_folder() {
    return std::string([[[NSBundle mainBundle] resourcePath] UTF8String]) + "/";
}