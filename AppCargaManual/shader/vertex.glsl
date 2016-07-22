in vec3 position;
in vec3 normal;
in vec2 texCoord;

position_ = MV * vec4(vVertex, 1.0);
normal_ = normalize(N * vNormal);
texture_ = vTexture;
//shadow_ = S * M * vec4(vVertex, 1.0);

gl_Position = MVP * vec4(vVertex, 1.0);

