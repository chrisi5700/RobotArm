#version 300 es
precision highp float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in mat4 instanceModel;
layout (location = 6) in vec3 instanceColor;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec3 v_Color;
out vec3 v_ViewPos;
out float v_Height;

void main()
{
    vec4 worldPos = instanceModel * vec4(aPos, 1.0);
    v_FragPos = worldPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(instanceModel)));
    v_Normal = normalize(normalMatrix * aNormal);

    v_Color = instanceColor;
    v_ViewPos = viewPos;
    v_Height = worldPos.y;

    gl_Position = projection * view * worldPos;
}