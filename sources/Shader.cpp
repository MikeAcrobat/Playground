//
//  Shader.cpp
//  3d technics
//
//  Created by Mike on 12/6/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#include "Shader.h"
#include "Utility.h"

const char * Shader::ATTRIBUTE_NAME_POSITION        = "v_position";
const char * Shader::ATTRIBUTE_NAME_TEXTURE_COORD   = "v_tex_coord";
const char * Shader::ATTRIBUTE_NAME_NORMAL          = "v_normal";
const char * Shader::ATTRIBUTE_NAME_TANGENT         = "v_tangent";
const char * Shader::ATTRIBUTE_NAME_BITANGENT       = "v_bitangent";
const char * Shader::ATTRIBUTE_NAME_COLOR           = "v_color";

bool Shader::compile(const std::string & code, GLenum type, GLuint & shader) {
    GLint status;
    
    if (code.empty()) return false;
    
    GL_DEBUG(shader = glCreateShader(type));
    
    GLchar * source = (GLchar *)code.data();
    
    GL_DEBUG(glShaderSource(shader, 1, (const GLchar**)&source, NULL));
    GL_DEBUG(glCompileShader(shader));
    
    GL_DEBUG(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
    
    if (!status) {
        GLint logLength = 0, charsWritten = 0;
        
        GL_DEBUG(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength));
        if (logLength > 1) {
            char * logBytes = (char*)malloc(logLength);
            GL_DEBUG(glGetShaderInfoLog(shader, logLength, &charsWritten, logBytes));
            printf("%s", logBytes);
            free(logBytes);
        }
        return false;
    }
    return (status == GL_TRUE);
}

void Shader::parse_attributes() {
    attributes.clear();
    
    GLint attributes_count, name_length;
    GL_DEBUG(glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTES, &attributes_count));
    if (attributes_count > 0) {
        GL_DEBUG(glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &name_length));
        if (name_length > 0) {
            static GLchar attribName[128];
            
            for (int i = 0; i < attributes_count; i++) {
                AttributeParameter attribute;
                GL_DEBUG(glGetActiveAttrib(shaderProgram, i, name_length, nullptr, &attribute.size, &attribute.type, attribName));
                attribName[name_length] = '\0';
                attribute.name = std::string(attribName);
                attribute.index = GL_DEBUG(glGetAttribLocation(shaderProgram, attribName));
                attributes[attribute.name] = attribute;
            }
        }
    }
}

void Shader::parse_uniforms() {
    uniforms.clear();
    
    GLint uniforms_count, name_length;
    GL_DEBUG(glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &uniforms_count));
    if (uniforms_count > 0) {
        GL_DEBUG(glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &name_length));
        if (name_length > 0) {
            static GLchar uniformName[128];
            
            for(int i = 0; i < uniforms_count; i++) {
                UniformParameter uniform;
                GL_DEBUG(glGetActiveUniform(shaderProgram, i, name_length, nullptr, &uniform.size, &uniform.type, uniformName));
                uniformName[name_length] = '\0';
                uniform.name = std::string(uniformName);
                uniform.location = GL_DEBUG(glGetUniformLocation(shaderProgram, uniformName));
                uniforms[uniform.name] = uniform;
            }
        }
    }
}

Shader::AttributeParameter * Shader::get_attribute(const std::string & name) {
    auto att_it = attributes.find(name);
    return att_it != attributes.end() ? &att_it->second : nullptr;
}

Shader::UniformParameter * Shader::get_uniform(const std::string & name) {
    auto uni_it = uniforms.find(name);
    return uni_it != uniforms.end() ? &uni_it->second : nullptr;
}

struct Pair {
    const char * name;
    GLint       index;
};

void Shader::bind_attributes() {
    static const std::vector<Pair> attributes = {
        { ATTRIBUTE_NAME_POSITION,          ATTRIBUTE_POSITION },
        { ATTRIBUTE_NAME_TEXTURE_COORD,     ATTRIBUTE_TEXTURE_COORD },
        { ATTRIBUTE_NAME_NORMAL,            ATTRIBUTE_NORMAL },
        { ATTRIBUTE_NAME_TANGENT,           ATTRIBUTE_TANGENT },
        { ATTRIBUTE_NAME_BITANGENT,         ATTRIBUTE_BITANGENT },
        { ATTRIBUTE_NAME_COLOR,             ATTRIBUTE_COLOR }
    };
    
    for (const auto & att : attributes) {
        GL_DEBUG(glBindAttribLocation(shaderProgram, att.index, att.name));
    }
}

Shader::Ptr Shader::load_from_files(const std::string & vs_file, const std::string & fs_file) {
    std::vector<char> vs_source;
	std::vector<char> fs_source;
    
	bool vs_load = Utility::load_file(vs_source, vs_file);
	bool fs_load = Utility::load_file(fs_source, fs_file);
    
	if (!vs_load || !fs_load) return nullptr;

    std::string vs_code(vs_source.data());
    std::string fs_code(fs_source.data());
    
    return std::make_shared<Shader>(vs_code, fs_code);
}

Shader::Shader(const std::string & vs_code, const std::string & fs_code) {
    shaderProgram = vertexShader = fragmentShader = 0;
    
    
    shaderProgram = GL_DEBUG(glCreateProgram());
	vertexShader = fragmentShader = 0;
    
    if (!compile(vs_code, GL_VERTEX_SHADER, vertexShader)) {
        printf("Failed to compile vertex program");
    }
    
    if (!compile(fs_code, GL_FRAGMENT_SHADER, fragmentShader)) {
        printf("Failed to compile fragment program");
    }
    
    if (vertexShader) {
        GL_DEBUG(glAttachShader(shaderProgram, vertexShader));
    }
    
    if (fragmentShader) {
        GL_DEBUG(glAttachShader(shaderProgram, fragmentShader));
    }
    
    bind_attributes();
    
    GL_DEBUG(glLinkProgram(shaderProgram));
    
    GL_DEBUG(glValidateProgram(shaderProgram));
    
    parse_attributes();
    parse_uniforms();
    
    GLint status = GL_TRUE;
    GL_DEBUG(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status));
    
    if (vertexShader) {
        GL_DEBUG(glDeleteShader(vertexShader));
    }
    
    if (fragmentShader) {
        GL_DEBUG(glDeleteShader(fragmentShader));
    }
    
    vertexShader = fragmentShader = 0;
    
    if (status == GL_FALSE) {
        printf("Failed to link program");
        GL_DEBUG(glDeleteProgram(shaderProgram));
        shaderProgram = 0;
    }
}

Shader::~Shader(void) {
    
    if (vertexShader) {
        GL_DEBUG(glDeleteShader(vertexShader));
    }
    
    if (fragmentShader) {
        GL_DEBUG(glDeleteShader(fragmentShader));
    }
    
    vertexShader = fragmentShader = 0;

    if (shaderProgram) {
        GL_DEBUG(glDeleteProgram(shaderProgram));
    }
    
    shaderProgram = 0;
}

void Shader::bind() {
    if (shaderProgram) {
        GL_DEBUG(glUseProgram(shaderProgram));
    }
}

/*
 The type argument will return a pointer to the uniform variable's data type. The symbolic constants GL_FLOAT, GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4, GL_INT, GL_INT_VEC2, GL_INT_VEC3, GL_INT_VEC4, GL_BOOL, GL_BOOL_VEC2, GL_BOOL_VEC3, GL_BOOL_VEC4, GL_FLOAT_MAT2, GL_FLOAT_MAT3, GL_FLOAT_MAT4, GL_SAMPLER_2D, or GL_SAMPLER_CUBE may be returned.
 */

bool Shader::inner_set_value(const std::string & name, const float * value, GLenum type, int count) {
    UniformParameter * uniform = get_uniform(name);
    if (uniform == nullptr) {
        //printf("can't find uniform with name %s \n", name.c_str());
        return false;
    }
    
    if (uniform->type != type) {
        printf("passing wrong type to %s \n", name.c_str());
        return false;
    }
    
    switch (type) {
        case GL_FLOAT:
            GL_DEBUG(glUniform1fv(uniform->location, count, value));
            break;
        case GL_FLOAT_VEC2:
            GL_DEBUG(glUniform2fv(uniform->location, count, value));
            break;
        case GL_FLOAT_VEC3:
            GL_DEBUG(glUniform3fv(uniform->location, count, value));
            break;
        case GL_FLOAT_VEC4:
            GL_DEBUG(glUniform4fv(uniform->location, count, value));
            break;
        case GL_FLOAT_MAT3:
            GL_DEBUG(glUniformMatrix3fv(uniform->location, count, GL_FALSE, value));
            break;
        case GL_FLOAT_MAT4:
            GL_DEBUG(glUniformMatrix4fv(uniform->location, count, GL_FALSE, value));
            break;
        default:
            printf("Passing this value is not implemented\n");
            break;
    }
    
    return true;
}

template<> bool Shader::set_value(const std::string & name, const glm::vec4 & value) {
    return inner_set_value(name, glm::value_ptr(value), GL_FLOAT_VEC4);
}

template<> bool Shader::set_value(const std::string & name, const glm::vec3 & value) {
    return inner_set_value(name, glm::value_ptr(value), GL_FLOAT_VEC3);
}

template<> bool Shader::set_value(const std::string & name, const glm::vec2 & value) {
    return inner_set_value(name, glm::value_ptr(value), GL_FLOAT_VEC2);
}

template<> bool Shader::set_value(const std::string & name, const glm::mat3 & value) {
    return inner_set_value(name, glm::value_ptr(value), GL_FLOAT_MAT3);
}

template<> bool Shader::set_value(const std::string & name, const glm::mat4 & value) {
    return inner_set_value(name, glm::value_ptr(value), GL_FLOAT_MAT4);
}

template<> bool Shader::set_value(const std::string & name, const float & value) {
    return inner_set_value(name, &value, GL_FLOAT);
}

bool Shader::bind_sampler(const std::string & name, int state) {
    UniformParameter * uniform = get_uniform(name);
    if (uniform == nullptr) return false;
    
    if (uniform->type != GL_SAMPLER_2D) {
        printf("can't bind %s \n", name.c_str());
    }
    
    GL_DEBUG(glUniform1i(uniform->location, state));
    
    return true;
}

Shader::Ptr ShaderPool::get_shader(ShaderPool::Type type) {
    static Shader::Ptr geom_shader = Shader::load_from_files("shaders/deferred_geometry.vs", "shaders/deferred_geometry.fs");
    static Shader::Ptr ssao_shader = Shader::load_from_files("shaders/ssao.vs", "shaders/ssao.fs");
    
    if (type == Geometry)   return geom_shader;
    if (type == SSAO_Pass)  return ssao_shader;
    
    return nullptr;
}