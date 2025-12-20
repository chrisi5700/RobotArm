#version 300 es
precision highp float;
precision highp int;
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec3 Color;
    vec3 ViewPos;
    float Height;
} fs_in;

out vec4 FragColor;

// Light parameters
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

// Material parameters (tweakable!)
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float shininess;

// Effect toggles and parameters
uniform bool enableRimLight;
uniform vec3 rimColor;
uniform float rimPower;
uniform float rimStrength;

uniform bool enableHeightFog;
uniform vec3 fogColor;
uniform float fogDensity;
uniform float fogHeightFalloff;

uniform bool enableGammaCorrection;
uniform float gamma;

// Fresnel for more realistic reflections
uniform bool enableFresnel;
uniform float fresnelBias;
uniform float fresnelScale;
uniform float fresnelPower;

void main()
{
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(fs_in.ViewPos - fs_in.FragPos);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);

    // Base color
    vec3 objectColor = fs_in.Color;

    // ===================
    // BLINN-PHONG LIGHTING
    // ===================

    // Ambient - fake global illumination
    vec3 ambient = ambientStrength * objectColor;

    // Diffuse - direct light hitting surface
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * NdotL * objectColor * lightColor * lightIntensity;

    // Specular - shiny highlights (Blinn-Phong uses half vector)
    vec3 halfDir = normalize(lightDir + viewDir);
    float NdotH = max(dot(normal, halfDir), 0.0);
    float spec = pow(NdotH, shininess);
    vec3 specular = specularStrength * spec * lightColor * lightIntensity;

    // Fresnel - edges reflect more (like real materials)
    if (enableFresnel) {
        float fresnel = fresnelBias + fresnelScale * pow(1.0 - max(dot(viewDir, normal), 0.0), fresnelPower);
        specular *= fresnel;
    }

    vec3 result = ambient + diffuse + specular;

    // ===================
    // RIM LIGHTING
    // ===================
    // Creates a glow around edges facing away from camera

    if (enableRimLight) {
        float rim = 1.0 - max(dot(viewDir, normal), 0.0);
        rim = pow(rim, rimPower);
        result += rimStrength * rim * rimColor;
    }

    // ===================
    // HEIGHT FOG
    // ===================
    // Fog that's thicker at lower heights

    if (enableHeightFog) {
        float distance = length(fs_in.ViewPos - fs_in.FragPos);
        float heightFactor = exp(-fs_in.Height * fogHeightFalloff);
        float fogFactor = 1.0 - exp(-distance * fogDensity * heightFactor);
        fogFactor = clamp(fogFactor, 0.0, 1.0);
        result = mix(result, fogColor, fogFactor);
    }

    // ===================
    // GAMMA CORRECTION
    // ===================
    // Converts from linear space to sRGB for correct display

    if (enableGammaCorrection) {
        result = pow(result, vec3(1.0 / gamma));
    }

    FragColor = vec4(result, 1.0);
}