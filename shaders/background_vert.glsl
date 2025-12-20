// background.vert
#version 300 es
precision highp float;
out vec2 TexCoord;

void main()
{
    // Fullscreen triangle trick - no VBO needed!
    vec2 positions[3] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 3.0, -1.0),
    vec2(-1.0,  3.0)
    );

    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
    TexCoord = positions[gl_VertexID] * 0.5 + 0.5;
}