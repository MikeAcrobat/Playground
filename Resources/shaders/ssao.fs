#version 330 core

uniform sampler2D depth_texture;
uniform sampler2D normal_texture;
uniform sampler2D noise;

in vec2 texture_uv;
in vec3 view_dir;

out vec4 color;

uniform float camera_near;
uniform float camera_far;

uniform mat4 projection;

uniform float sample_radius;
uniform float strength;
uniform float falloff;

float get_depth(in vec2 uv) {
	float depth = texture(depth_texture, uv).x * 2.0 - 1.0;
    return 2.0 * camera_far * camera_near / (camera_far + camera_near - depth * (camera_far - camera_near));
}

vec3 get_normal(in vec2 uv) {
    return texture(normal_texture, uv).xyz * 2.0 - vec3(1, 1, 1);
}

vec2 project_position(in vec3 position) {
    vec4 uv = projection * vec4(position, 1.0);
    uv.xy /= uv.w;
    uv.xy = uv.xy * 0.5 + vec2(0.5);
    return uv.xy;
}

void main(void)
{
    vec3 pSphere[16] = vec3[](vec3(0.53812504,   0.18565957,  -0.43192),
                              vec3(0.13790712,   0.24864247,   0.44301823),
                              vec3(0.33715037,   0.56794053,  -0.005789503),
                              vec3(-0.6999805,  -0.04511441,  -0.0019965635),
                              vec3(0.06896307,  -0.15983082,  -0.85477847),
                              vec3(0.056099437,  0.006954967, -0.1843352),
                              vec3(-0.014653638, 0.14027752,   0.0762037),
                              vec3(0.010019933, -0.1924225,   -0.034443386),
                              vec3(-0.35775623, -0.5301969,   -0.43581226),
                              vec3(-0.3169221,   0.106360726,  0.015860917),
                              vec3(0.010350345, -0.58698344,   0.0046293875),
                              vec3(-0.08972908, -0.49408212,   0.3287904),
                              vec3(0.7119986,   -0.015469003, -0.09183723),
                              vec3(-0.053382345, 0.059675813, -0.5411899),
                              vec3(0.035267662, -0.063188605,  0.54602677),
                              vec3(-0.47761092,  0.2847911,   -0.0271716));
    
    float fragment_depth = get_depth(texture_uv);
    vec3 screen_space_position = view_dir * fragment_depth;
    
    vec3 normal = get_normal(texture_uv);
    
    float occlusion = 0;
    
    vec3 rvec = texture(noise, texture_uv).xyz * 2.0 - 1.0;
    vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);
    
    for (int i = 0; i < 32; ++i) {

        vec3 offset = sample_radius * normalize(tbn * pSphere[i]);
        
        vec3 point = screen_space_position + offset * sign(dot(offset, normal));
        
        vec2 point_uv = project_position(point);
        
        float point_depth = get_depth(point_uv.xy);
        
        float difference = fragment_depth - point_depth;
        
        float normDiff = 1.0 - dot(normal, get_normal(point_uv.xy));
        
        occlusion += step(falloff, difference) * normDiff * (1.0 - smoothstep(falloff, strength, difference));
    }
    
    float ao = 1.0 - occlusion / 16.f;
    color = vec4(ao, ao, ao, 1.0);
}