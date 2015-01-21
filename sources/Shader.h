//
//  Shader.h
//  3d technics
//
//  Created by Mike on 12/6/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#ifndef ___d_technics__Shader__
#define ___d_technics__Shader__

class Shader {
    GLuint shaderProgram, vertexShader, fragmentShader;
    
    static bool compile(const std::string & code, GLenum type, GLuint & shader);
    
    struct AttributeParameter {
        GLuint      index;
        GLint       size;
        GLenum      type;
        std::string name;
    };
    
    struct UniformParameter {
        GLint       location;
        GLint       size;
        GLenum      type;
        std::string name;
    };

    std::unordered_map<std::string, AttributeParameter> attributes;
    std::unordered_map<std::string, UniformParameter>   uniforms;
    
    void parse_attributes();
    void parse_uniforms();
    
    AttributeParameter *    get_attribute(const std::string & name);
    UniformParameter *      get_uniform(const std::string & name);

    void bind_attributes();
    
    bool inner_set_value(const std::string & name, const float * value, GLenum type, int count = 1);

public:
    
    typedef std::shared_ptr<Shader> Ptr;
    
    enum Attributes {
        ATTRIBUTE_POSITION,
        ATTRIBUTE_TEXTURE_COORD,
        ATTRIBUTE_NORMAL,
        ATTRIBUTE_TANGENT,
        ATTRIBUTE_BITANGENT,
        ATTRIBUTE_COLOR,
    };
    
    static const char * ATTRIBUTE_NAME_POSITION;
    static const char * ATTRIBUTE_NAME_TEXTURE_COORD;
    static const char * ATTRIBUTE_NAME_NORMAL;
    static const char * ATTRIBUTE_NAME_TANGENT;
    static const char * ATTRIBUTE_NAME_BITANGENT;
    static const char * ATTRIBUTE_NAME_COLOR;
    
    static Shader::Ptr load_from_files(const std::string & vs_file, const std::string & fs_code);
    
    Shader(const std::string & vs_code, const std::string & fs_code);
    ~Shader(void);
    
    void bind();
    
    template<typename T>
    bool set_value(const std::string & name, const T & value);
    
    bool bind_sampler(const std::string & name, int state);
};

class ShaderPool {
public:
    enum Type {
        Geometry,
        SSAO_Pass,
    };

    static Shader::Ptr get_shader(Type type);
};

#endif /* defined(___d_technics__Shader__) */
