#version 330 core

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;

uniform float specular_shininess;

in vec2 texture_uv;
in vec3 world_normal;
in vec3 world_tangent;
in vec3 world_bitangent;

layout (location = 0) out vec4 result_color;
layout (location = 1) out vec4 result_normal;
layout (location = 2) out vec4 result_specular;

vec3 calc_normal() {
    return texture(normal_texture, texture_uv).xyz * 2.f - vec3(1, 1, 1);
}

void main(void)
{
    float spec = texture(specular_texture, texture_uv).r;
    result_specular = vec4(spec, specular_shininess, 0, 1);
    result_color    = texture(diffuse_texture, texture_uv);
    
    mat3 tbn = mat3(normalize(world_tangent), normalize(world_bitangent), normalize(world_normal));
    result_normal = vec4((tbn * calc_normal() + vec3(1, 1, 1)) / 2.0, 1.f);
}