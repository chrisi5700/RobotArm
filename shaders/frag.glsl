#version 300 es
precision highp float;
precision highp int;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec3 v_Color;
in vec3 v_ViewPos;
in float v_Height;

out vec4 FragColor;

// Light
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

// Cel shading
uniform float shadowThreshold;    // Where shadow starts (0.0-0.5)
uniform float shadowSoftness;     // Transition width
uniform vec3 shadowTint;          // Multiplied with base color in shadow
uniform float shadowStrength;     // How dark shadows are (0-1)

// Specular highlight
uniform bool enableSpecular;
uniform float specularThreshold;
uniform float specularSize;       // Smaller = tighter highlight

// Rim light
uniform bool enableRimLight;
uniform vec3 rimColor;
uniform float rimThreshold;
uniform float rimSoftness;

// Edge darkening
uniform bool enableOutline;
uniform float outlineThreshold;
uniform float outlineStrength;

// Fog
uniform bool enableHeightFog;
uniform vec3 fogColor;
uniform float fogDensity;
uniform float fogHeightFalloff;

// Gamma
uniform bool enableGammaCorrection;
uniform float gamma;

void main()
{
    vec3 normal = normalize(v_Normal);
    vec3 viewDir = normalize(v_ViewPos - v_FragPos);
    vec3 lightDir = normalize(lightPos - v_FragPos);

    vec3 baseColor = v_Color;

    // ===================
    // CEL SHADING - shadow/lit split
    // ===================
    float NdotL = dot(normal, lightDir);

    // Smooth threshold between shadow and lit
    float lit = smoothstep(shadowThreshold - shadowSoftness,
    shadowThreshold + shadowSoftness, NdotL);

    // Shadow color is base color darkened and tinted
    vec3 shadowColor = baseColor * shadowTint * (1.0 - shadowStrength);
    vec3 litColor = baseColor * lightColor * lightIntensity;

    vec3 result = mix(shadowColor, litColor, lit);

    // ===================
    // SPECULAR - small hard highlight
    // ===================
    if (enableSpecular) {
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = max(dot(viewDir, reflectDir), 0.0);
        spec = pow(spec, specularSize);
        float specMask = smoothstep(specularThreshold - 0.02, specularThreshold + 0.02, spec);
        // Additive highlight, but capped
        result += specMask * lightColor * 0.3 * lit; // Only show on lit side
    }

    // ===================
    // RIM LIGHT
    // ===================
    if (enableRimLight) {
        float rim = 1.0 - max(dot(viewDir, normal), 0.0);
        float rimMask = smoothstep(rimThreshold - rimSoftness,
        rimThreshold + rimSoftness, rim);
        // Rim shows more on lit side for that backlit look
        float rimLit = mix(0.3, 1.0, lit);
        result = mix(result, rimColor, rimMask * rimLit * 0.5);
    }

    // ===================
    // EDGE DARKENING
    // ===================
    if (enableOutline) {
        float edge = 1.0 - max(dot(viewDir, normal), 0.0);
        float outline = smoothstep(outlineThreshold, outlineThreshold + 0.1, edge);
        result *= 1.0 - (outline * outlineStrength);
    }

    // ===================
    // HEIGHT FOG
    // ===================
    if (enableHeightFog) {
        float distance = length(v_ViewPos - v_FragPos);
        float heightFactor = exp(-v_Height * fogHeightFalloff);
        float fogFactor = 1.0 - exp(-distance * fogDensity * heightFactor);
        fogFactor = clamp(fogFactor, 0.0, 1.0);
        result = mix(result, fogColor, fogFactor);
    }

    // Gamma
    if (enableGammaCorrection) {
        result = pow(result, vec3(1.0 / gamma));
    }

    FragColor = vec4(result, 1.0);
}