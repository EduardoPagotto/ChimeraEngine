#version 440 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2; // cor que sera o entity (este é o segundo buffer o 0 é o screem)
                                     // ou passar pelo uniform no calldraw

uniform vec4 colour;
uniform vec2 light_pos;

in DATA {
    vec4 position;
    vec2 uv;
    float tid;
    vec4 color;
}
fs_in;

uniform sampler2D textures[32];

void main() {
    float intensity = 1.0 / length(fs_in.position.xy - light_pos);

    vec4 textColor = fs_in.color;
    if (fs_in.tid > 0.0) {
        int tid = int(fs_in.tid - 0.5);
        // textColor = texture(textures[tid], fs_in.uv);
        textColor = fs_in.color * texture(textures[tid], fs_in.uv); // color in texture, write didnt change
    }

    color = textColor * intensity;
    color2 = 50;
}
