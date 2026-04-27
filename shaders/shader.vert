#version 460

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 model_inverse_transpose;
    mat4 view;
    mat4 projection;
    vec3 eye;
};

struct Transform {
    mat4 model_matrix;
    mat4 model_inverse_transpose_matrix;
};

layout(binding = 2) readonly buffer TransformBuffer {
    Transform transforms[];
};

layout(binding = 3) readonly buffer ModelMatrixFirstIndexBuffer {
    uint model_matrix_first_indices[];
};

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;
layout(location = 3) in vec4 in_tangent;
layout(location = 4) in uint in_material_index;

layout(location = 0) out vec3 out_position; // in world space.
layout(location = 1) out vec2 out_tex_coord;
layout(location = 2) out vec3 out_tangent; // in world space.
layout(location = 3) out vec3 out_binormal; // in world space.
layout(location = 4) out vec3 out_normal; // in world space.
layout(location = 5) out flat uint out_material_index;

void main() {
    const uint transform_index = model_matrix_first_indices[gl_DrawID] + gl_InstanceIndex;
    const mat4 model_matrix = transforms[transform_index].model_matrix;
    const mat4 model_inverse_transpose_matrix = transforms[transform_index].model_inverse_transpose_matrix;
    vec4 position = model_matrix * vec4(in_position, 1.0);
    gl_Position = projection * view * position;
    out_position = position.xyz;
    out_tex_coord = in_tex_coord;
    out_normal = normalize(mat3(model_inverse_transpose_matrix) * in_normal);
    out_tangent = normalize(mat3(model_matrix) * in_tangent.xyz);
    // Tangent handedness is given by tangent's w component.
    out_binormal = cross(out_normal, out_tangent) * in_tangent.w;
    out_material_index = in_material_index;
}
