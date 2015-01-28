#version 330 core

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture;

in vec2 texture_uv;

out vec4 result_color;

//vec3 calc_normal() {
//    return texture(normal_texture, texture_uv).xyz * 2.f - vec3(1, 1, 1);
//}

void main(void)
{
    //result_color = vec4(texture_uv.xy, 0, 1);
    //result_color = vec4(calc_normal(), 1);
    result_color = vec4(texture(normal_texture, texture_uv).xyz, 1);
}