#version 300 es
precision highp float;
precision highp int;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec3 v_Color;
in vec3 v_ViewPos;
in float v_Height;

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
    vec3 normal = normalize(v_Normal);
    vec3 viewDir = normalize(v_ViewPos - v_FragPos);
    vec3 lightDir = normalize(lightPos - v_FragPos);

    vec3 objectColor = v_Color;

    // Ambient
    vec3 ambient = ambientStrength * objectColor;

    // Diffuse
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * NdotL * objectColor * lightColor * lightIntensity;

    // Specular (Blinn-Phong)
    vec3 halfDir = normalize(lightDir + viewDir);
    float NdotH = max(dot(normal, halfDir), 0.0);
    float spec = pow(NdotH, shininess);
    vec3 specular = specularStrength * spec * lightColor * lightIntensity;

    // Fresnel
    if (enableFresnel) {
        float fresnel = fresnelBias + fresnelScale * pow(1.0 - max(dot(viewDir, normal), 0.0), fresnelPower);
        specular *= fresnel;
    }

    vec3 result = ambient + diffuse + specular;

    // Rim lighting
    if (enableRimLight) {
        float rim = 1.0 - max(dot(viewDir, normal), 0.0);
        rim = pow(rim, rimPower);
        result += rimStrength * rim * rimColor;
    }

    // Height fog
    if (enableHeightFog) {
        float distance = length(v_ViewPos - v_FragPos);
        float heightFactor = exp(-v_Height * fogHeightFalloff);
        float fogFactor = 1.0 - exp(-distance * fogDensity * heightFactor);
        fogFactor = clamp(fogFactor, 0.0, 1.0);
        result = mix(result, fogColor, fogFactor);
    }

    // Gamma correction
    if (enableGammaCorrection) {
        result = pow(result, vec3(1.0 / gamma));
    }

    FragColor = vec4(result, 1.0);
}
