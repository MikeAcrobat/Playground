//
//  pch.pch
//  3d technics
//
//  Created by Mike on 12/11/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#ifndef _d_technics_pch_pch
#define _d_technics_pch_pch

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
#include <unordered_map>

#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utility.h"

#include "Camera.h"

#include "Vertex.h"

#include "Textures/Texture.h"

#include "Textures/RenderTarget.h"

#include "Shader.h"

#include "Material.h"

#include "MeshBuffer.h"

#include "Mesh.h"

#include "Renderer.h"

#endif
