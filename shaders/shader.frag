#version 450
#extension GL_EXT_nonuniform_qualifier : enable

struct Material {
    uvec4 textures; // x = diffuse index, y = roughness index, z = normal index, w = occlusion index.
    vec4 base_color_factor;
    vec4 metallic_roughness_occlusion_factor;
};

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 model_inverse_transpose;
    mat4 view;
    mat4 projection;
    vec3 eye;
};

layout(binding = 1) readonly buffer MaterialBuffer {
    Material materials[];
};

// We can alias bindless textures of different types.
layout(set = 1, binding = 0) uniform sampler2D textures_2d[];
layout(set = 1, binding = 0) uniform sampler3D textures_3d[];

layout(location = 0) in vec3 in_position; // in world space.
layout(location = 1) in vec2 in_tex_coord;
layout(location = 2) in vec3 in_tangent; // in world space.
layout(location = 3) in vec3 in_binormal; // in world space.
layout(location = 4) in vec3 in_normal; // in world space.
layout(location = 5) in flat uint in_material_index;

layout(location = 0) out vec4 out_color;

#define PI 3.1415926538
#define INVALID_TEXTURE_INDEX 65535

vec3 decodeSrgb(vec3 c) {
    vec3 result;
    if (c.r <= 0.04045) {
        result.r = c.r / 12.92;
    } else {
        result.r = pow((c.r + 0.055) / 1.055, 2.4);
    }
    if ( c.g <= 0.04045) {
        result.g = c.g / 12.92;
    } else {
        result.g = pow((c.g + 0.055) / 1.055, 2.4);
    }
    if ( c.b <= 0.04045) {
        result.b = c.b / 12.92;
    } else {
        result.b = pow((c.b + 0.055) / 1.055, 2.4);
    }
    return clamp(result, 0.0, 1.0);
}

vec3 encodeSrgb(vec3 c) {
    vec3 result;
    if (c.r <= 0.0031308) {
        result.r = c.r * 12.92;
    } else {
        result.r = 1.055 * pow(c.r, 1.0 / 2.4) - 0.055;
    }
    if (c.g <= 0.0031308) {
        result.g = c.g * 12.92;
    } else {
        result.g = 1.055 * pow(c.g, 1.0 / 2.4) - 0.055;
    }
    if (c.b <= 0.0031308) {
        result.b = c.b * 12.92;
    } else {
        result.b = 1.055 * pow(c.b, 1.0 / 2.4) - 0.055;
    }
    return clamp(result, 0.0, 1.0);
}

float heaviside(float x) {
    if ( x > 0.0 ) return 1.0;
    else return 0.0;
}

void main() {
    const uvec4 textures = materials[in_material_index].textures;
    const vec4 base_color_factor = materials[in_material_index].base_color_factor;
    const vec4 metallic_roughness_occlusion_factor = materials[in_material_index].metallic_roughness_occlusion_factor;
    vec4 base_color = base_color_factor;
    if (textures.x != INVALID_TEXTURE_INDEX) {
        base_color *= texture(textures_2d[nonuniformEXT(textures.x)], in_tex_coord);
    }

    vec3 tangent = normalize(in_tangent);
    vec3 binormal = normalize(in_binormal);
    vec3 normal = normalize(in_normal);

    if (gl_FrontFacing == false)
    {
        tangent *= -1.0;
        binormal *= -1.0;
        normal *= -1.0;
    }

    if (textures.z != INVALID_TEXTURE_INDEX) {
        // Build tangent space to world space transformation matrix. Remember,
        // tangent, binormal and normal vectors are already in world space.
        mat3 tangent_to_world = mat3(tangent, binormal, normal);
        // Compute normal vector
        vec3 normal_map = texture(textures_2d[nonuniformEXT(textures.z)], in_tex_coord).xyz;
        normal_map = normal_map * 2.0 - 1.0;
        normal = normalize(tangent_to_world * normal_map);
    }

    const vec3 light_position = vec3(5.0, 5.0, 5.0);
    const float light_intensity = 80.0;
    const float light_range = 20.0;
    // Exception for variable naming standards here just because this is a classic way to name the
    // vectors.
    vec3 V = normalize(eye - in_position);
    vec3 L = normalize(light_position - in_position);
    vec3 N = normal;
    vec3 H = normalize(L + V);

    float NdotL = clamp(dot(N, L), 0, 1);
    float NdotV = clamp(dot(N, V), 0, 1);
    float HdotL = clamp(dot(H, L), 0, 1);
    float HdotV = clamp(dot(H, V), 0, 1);
    float NdotH = clamp(dot(N, H), 0, 1);

    float metalness = metallic_roughness_occlusion_factor.x;
    float roughness = metallic_roughness_occlusion_factor.y;

    if (textures.y != INVALID_TEXTURE_INDEX) {
        vec4 metallic_roughness_texture = texture(textures_2d[nonuniformEXT(textures.y)], in_tex_coord);
        roughness *= metallic_roughness_texture.g;
        metalness *= metallic_roughness_texture.b;
    }

    float occlusion = metallic_roughness_occlusion_factor.z;
    if (textures.w != INVALID_TEXTURE_INDEX) {
        vec4 occlusion_texture = texture(textures_2d[nonuniformEXT(textures.w)], in_tex_coord);
        occlusion *= occlusion_texture.r;
    }

    base_color.rgb = decodeSrgb(base_color.rgb);

    // https://www.khronos.org/registry/glTF/specs/2.0/glTF-2.0.html#specular-brdf
    float alpha = pow(roughness, 2.0);
    float alpha_squared = alpha * alpha;
    float d_denom = (NdotH * NdotH) * (alpha_squared - 1.0) + 1.0;
    float distribution = (alpha_squared * heaviside(NdotH)) / (PI * d_denom * d_denom);

    float distance = length(light_position - in_position);
    float intensity = light_intensity * max(min(1.0 - pow(distance / light_range, 4.0), 1.0), 0.0) / pow(distance, 2.0);

    vec3 result = vec3(0, 0, 0);
    if (NdotL > 0.0 || NdotV > 0.0)
    {
        float visibility = (heaviside(HdotL) / (abs(NdotL) + sqrt(alpha_squared + (1.0 - alpha_squared) * (NdotL * NdotL)))) *
            (heaviside(HdotV) / (abs(NdotV) + sqrt(alpha_squared + (1.0 - alpha_squared) * (NdotV * NdotV))));
        float specular_brdf = intensity * NdotL * visibility * distribution;
        vec3 diffuse_brdf = intensity * NdotL * (1.0 / PI) * base_color.rgb;

        // NOTE(marco): f0 in the formula notation refers to the base colour here
        vec3 conductor_fresnel = specular_brdf * (base_color.rgb + (1.0 - base_color.rgb) * pow(1.0 - abs(HdotV), 5));

        // NOTE(marco): f0 in the formula notation refers to the value derived from ior = 1.5
        float f0 = 0.04; // pow((1 - ior) / (1 + ior), 2)
        float fr = f0 + (1 - f0) * pow(1 - abs(HdotV), 5);
        vec3 fresnel_mix = mix(diffuse_brdf, vec3(specular_brdf), fr);

        result = mix(fresnel_mix, conductor_fresnel, metalness);

    }
    out_color = vec4(encodeSrgb(result), base_color.a);
}
