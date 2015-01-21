#version 330 core

in vec3 v_position;
in vec2 v_tex_coord;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;

uniform mat4 world;
uniform mat3 normal_matrix;
uniform mat4 view_projection;

out vec2 texture_uv;
out vec3 world_normal;
out vec3 world_tangent;
out vec3 world_bitangent;

void main(void)
{
    texture_uv              = v_tex_coord;
    world_normal            = normalize(mat3(world) * normal_matrix * v_normal);
    world_tangent           = normalize(mat3(world) * normal_matrix * v_tangent);
    world_bitangent         = normalize(mat3(world) * normal_matrix * v_bitangent);
    gl_Position             = view_projection * world * vec4(v_position, 1.0);
}