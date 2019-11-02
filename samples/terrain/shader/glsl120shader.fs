#version 120

uniform sampler2D ShadowMap, RotationTexture;

uniform vec3 LightDirection;
uniform float Scale, Radius;

varying vec3 var_Normal;

vec2 PoissonDisk[16] =
    vec2[](vec2(-0.94201624, -0.39906216), vec2(0.94558609, -0.76890725), vec2(-0.094184101, -0.92938870),
           vec2(0.34495938, 0.29387760), vec2(-0.91588581, 0.45771432), vec2(-0.81544232, -0.87912464),
           vec2(-0.38277543, 0.27676845), vec2(0.97484398, 0.75648379), vec2(0.44323325, -0.97511554),
           vec2(0.53742981, -0.47373420), vec2(-0.26496911, -0.41893023), vec2(0.79197514, 0.19090188),
           vec2(-0.24188840, 0.99706507), vec2(-0.81409955, 0.91437590), vec2(0.19984126, 0.78641367),
           vec2(0.14383161, -0.14100790));

void main() {
    vec3 ShadowTexCoord = gl_TexCoord[0].xyz / gl_TexCoord[0].w;

    ShadowTexCoord.z -= 0.005;

    vec4 ScaleRotationVector = (texture2D(RotationTexture, gl_FragCoord.st * Scale) * 2.0 - 1.0) * Radius;

    mat2x2 ScaleRotationMatrix = mat2x2(ScaleRotationVector.xy, ScaleRotationVector.zw);

    float Shadow = 0.0;

    for (int i = 0; i < 16; i++) {
        float Depth = texture2D(ShadowMap, ShadowTexCoord.st + ScaleRotationMatrix * PoissonDisk[i]).r;

        if (ShadowTexCoord.z < Depth) {
            Shadow += 1.0;
        }
    }

    Shadow /= 16.0;

    vec3 Normal = normalize(var_Normal);

    float NdotLD = max(0.0, dot(Normal, LightDirection));

    gl_FragColor = vec4(gl_Color.rgb * (0.25 + 0.75 * NdotLD * Shadow), 1.0);
}
