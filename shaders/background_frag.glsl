#version 300 es
precision highp float;
precision highp int;
in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 topColor;
uniform vec3 bottomColor;
uniform float gradientOffset;    // Shift gradient up/down
uniform float gradientExponent;  // Control falloff curve

void main()
{
    float t = TexCoord.y + gradientOffset;
    t = clamp(t, 0.0, 1.0);
    t = pow(t, gradientExponent);

    vec3 color = mix(bottomColor, topColor, t);
    FragColor = vec4(color, 1.0);
}