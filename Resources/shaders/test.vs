#version 330 core

in vec3 v_position;
in vec2 v_tex_coord;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;

out vec2 texture_uv;

void main(void)
{
    texture_uv              = v_tex_coord;
    gl_Position             = vec4(v_position.xy, 1.0, 1.0);
}