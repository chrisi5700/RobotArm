#version 300 es
precision highp float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in mat4 instanceModel;
layout (location = 6) in vec3 instanceColor;

uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec3 Color;
    vec3 ViewPos;    // Camera position in world space
    float Height;    // For height-based effects
} vs_out;

uniform vec3 viewPos;

void main()
{
    vec4 worldPos = instanceModel * vec4(aPos, 1.0);
    vs_out.FragPos = worldPos.xyz;

    // Normal matrix - handles non-uniform scaling correctly
    mat3 normalMatrix = transpose(inverse(mat3(instanceModel)));
    vs_out.Normal = normalize(normalMatrix * aNormal);

    vs_out.Color = instanceColor;
    vs_out.ViewPos = viewPos;
    vs_out.Height = worldPos.y;

    gl_Position = projection * view * worldPos;
}