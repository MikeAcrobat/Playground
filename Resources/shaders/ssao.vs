#version 330 core

in vec3 v_position;
in vec2 v_tex_coord;

out vec2 texture_uv;
out vec3 view_dir;

uniform mat4 projection;

void main(void)
{
    vec4 dir = inverse(projection) * vec4(v_position.xy, -1, 1);
    view_dir = dir.xyz;
    
    texture_uv = v_tex_coord;
    gl_Position = vec4(v_position, 1);
}