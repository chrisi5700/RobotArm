// floor.frag
#version 300 es
precision highp float;
precision highp int;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;

// Grid parameters
uniform vec3 gridColorA;      // Main grid color
uniform vec3 gridColorB;      // Secondary grid color
uniform float gridScale;      // Size of grid cells
uniform float gridLineWidth;  // Thickness of lines
uniform float gridFadeStart;  // Distance where fade begins
uniform float gridFadeEnd;    // Distance where grid is invisible

// Axis highlight
uniform bool showAxisLines;
uniform vec3 xAxisColor;
uniform vec3 zAxisColor;
uniform float axisLineWidth;

void main()
{
    vec2 coord = FragPos.xz * gridScale;

    // Grid pattern using derivatives for anti-aliased lines
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    float line = min(grid.x, grid.y);
    float gridPattern = 1.0 - min(line, 1.0);

    // Thicker lines every N units (major grid)
    vec2 majorCoord = FragPos.xz * gridScale * 0.1;  // Every 10 units
    vec2 majorGrid = abs(fract(majorCoord - 0.5) - 0.5) / fwidth(majorCoord);
    float majorLine = min(majorGrid.x, majorGrid.y);
    float majorPattern = 1.0 - min(majorLine, 1.0);

    // Combine minor and major grid
    float finalGrid = max(gridPattern * 0.3, majorPattern * 0.6);

    // Axis lines
    if (showAxisLines) {
        float xAxis = 1.0 - smoothstep(0.0, axisLineWidth / gridScale, abs(FragPos.z));
        float zAxis = 1.0 - smoothstep(0.0, axisLineWidth / gridScale, abs(FragPos.x));

        if (xAxis > 0.5) {
            FragColor = vec4(xAxisColor, 1.0);
            return;
        }
        if (zAxis > 0.5) {
            FragColor = vec4(zAxisColor, 1.0);
            return;
        }
    }

    // Distance fade
    float dist = length(viewPos.xz - FragPos.xz);
    float fade = 1.0 - smoothstep(gridFadeStart, gridFadeEnd, dist);

    // Base floor color with grid overlay
    vec3 floorColor = mix(gridColorA, gridColorB, finalGrid * fade);

    // Simple lighting for floor
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0) * 0.3 + 0.7;  // Mostly ambient

    FragColor = vec4(floorColor * diff, 1.0);
}