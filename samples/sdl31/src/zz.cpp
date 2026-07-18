#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <vector>

// 1. Definição da estrutura de vértice compatível
struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 uv;
};

// Exemplo de dados de inicialização
const std::vector<Vertex> vertices = {{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
                                      {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
                                      {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                                      {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}};

const std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};

void RenderMesh(SDL_GPUDevice* device, SDL_GPUGraphicsPipeline* pipeline) {

    // --- PASSO 1: CRIAR OS BUFFERS (VBO e IBO) ---

    SDL_GPUBufferCreateInfo vboInfo{};
    vboInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    vboInfo.size = vertices.size() * sizeof(Vertex);
    SDL_GPUBuffer* vertexBuffer = SDL_CreateGPUBuffer(device, &vboInfo);

    SDL_GPUBufferCreateInfo iboInfo{};
    iboInfo.usage = SDL_GPU_BUFFERUSAGE_INDEX;
    iboInfo.size = indices.size() * sizeof(uint32_t);
    SDL_GPUBuffer* indexBuffer = SDL_CreateGPUBuffer(device, &iboInfo);

    // --- PASSO 2: TRANSFERIR DADOS USANDO TRANSFER BUFFER (STAGING) ---

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transferInfo.size = vboInfo.size + iboInfo.size;
    SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    // Mapear e copiar os dados para o buffer de transferência
    uint8_t* mapPtr = (uint8_t*)SDL_MapGPUTransferBuffer(device, transferBuffer, false);
    SDL_memcpy(mapPtr, vertices.data(), vboInfo.size);
    SDL_memcpy(mapPtr + vboInfo.size, indices.data(), iboInfo.size);
    SDL_UnmapGPUTransferBuffer(device, transferBuffer);

    // Executar a cópia via Command Buffer
    SDL_GPUCommandBuffer* uploadCmd = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmd);

    SDL_GPUTransferBufferLocation vboSource{transferBuffer, 0};
    SDL_GPUBufferRegion vboDest{vertexBuffer, 0, vboInfo.size};
    SDL_UploadToGPUBuffer(copyPass, &vboSource, &vboDest, false);

    SDL_GPUTransferBufferLocation iboSource{transferBuffer, vboInfo.size};
    SDL_GPUBufferRegion iboDest{indexBuffer, 0, iboInfo.size};
    SDL_UploadToGPUBuffer(copyPass, &iboSource, &iboDest, false);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(uploadCmd);
    SDL_ReleaseGPUTransferBuffer(device, transferBuffer);

    // --- PASSO 3: CONFIGURAR OS ATTRIBUTES NA CRIAÇÃO DA PIPELINE ---
    /*
       Nota: O trecho abaixo demonstra como os atributos devem ser definidos na struct
       SDL_GPUGraphicsPipelineCreateInfo ao criar a sua pipeline principal.
    */
    SDL_GPUVertexAttribute vertexAttributes[4];

    // Posição (glm::vec3)
    vertexAttributes[0].location = 0;
    vertexAttributes[0].buffer_slot = 0;
    vertexAttributes[0].format = SDL_GPU_VERTEXFORMAT_FLOAT3;
    vertexAttributes[0].offset = offsetof(Vertex, pos);

    // Cor (glm::vec3)
    vertexAttributes[1].location = 1;
    vertexAttributes[1].buffer_slot = 0;
    vertexAttributes[1].format = SDL_GPU_VERTEXFORMAT_FLOAT3;
    vertexAttributes[1].offset = offsetof(Vertex, color);

    // Normal (glm::vec3)
    vertexAttributes[2].location = 2;
    vertexAttributes[2].buffer_slot = 0;
    vertexAttributes[2].format = SDL_GPU_VERTEXFORMAT_FLOAT3;
    vertexAttributes[2].offset = offsetof(Vertex, normal);

    // UV (glm::vec2)
    vertexAttributes[3].location = 3;
    vertexAttributes[3].buffer_slot = 0;
    vertexAttributes[3].format = SDL_GPU_VERTEXFORMAT_FLOAT2;
    vertexAttributes[3].offset = offsetof(Vertex, uv);

    SDL_GPUVertexBufferDescription bufferDesc{};
    bufferDesc.slot = 0;
    bufferDesc.pitch = sizeof(Vertex);
    bufferDesc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    bufferDesc.instance_step_rate = 0;

    // --- PASSO 4: VÍNCULO NO PIPELINE E DESENHO (RENDER LOOP) ---

    // No seu laço de renderização, ao obter o comando e iniciar o Render Pass:
    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
    // (Configuração prévia de texturas/alvos omitida por brevidade...)
    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmd, nullptr, 0, nullptr);

    SDL_BindGPUGraphicsPipeline(renderPass, pipeline);

    // Passando o VBO (Slot 0)
    SDL_GPUBufferBinding vboBinding{vertexBuffer, 0};
    SDL_BindGPUVertexBuffers(renderPass, 0, &vboBinding, 1);

    // Passando o IBO
    SDL_GPUBufferBinding iboBinding{indexBuffer, 0};
    SDL_BindGPUIndexBuffer(renderPass, &iboBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);

    // Desenhar os elementos indexados
    SDL_DrawGPUIndexedPrimitives(renderPass, indices.size(), 1, 0, 0, 0);

    SDL_EndGPURenderPass(renderPass);
    SDL_SubmitGPUCommandBuffer(cmd);

    // Limpeza (Lembre-se de liberar quando fechar a aplicação)
    // SDL_ReleaseGPUBuffer(device, vertexBuffer);
    // SDL_ReleaseGPUBuffer(device, indexBuffer);
}

//----
#version 450

// Atributos de entrada mapeados exatamente como na struct Vertex (C++)
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

// Saídas para o Fragment Shader
layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragUV;

void main() {
    // Exemplo básico passando as coordenadas e cores à frente
    gl_Position = vec4(inPos, 1.0);
    fragColor = inColor;
    fragUV = inUV;
}

//----

#include <SDL3/SDL.h>
#include <fstream>
#include <vector>

SDL_GPUShader* LoadSPIRVShader(SDL_GPUDevice* device, const char* filepath, SDL_GPUShaderStage stage) {
    // 1. Ler o arquivo binário SPIR-V do disco
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        SDL_Log("Falha ao abrir o shader SPIR-V: %s", filepath);
        return nullptr;
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<uint8_t> buffer(fileSize);
    file.seekg(0);
    file.read((char*)buffer.data(), fileSize);
    file.close();

    // 2. Configurar a criação do shader na SDL3 para Vulkan/SPIR-V
    SDL_GPUShaderCreateInfo shaderInfo{};
    shaderInfo.code_size = buffer.size();           // Tamanho em bytes do SPIR-V
    shaderInfo.code = buffer.data();                // Ponteiro para o binário bruto
    shaderInfo.entrypoint = "main";                 // Nome da função principal no GLSL
    shaderInfo.format = SDL_GPU_SHADERFORMAT_SPIRV; // Especifica o formato Vulkan SPIR-V
    shaderInfo.stage = stage;                       // SDL_GPU_SHADERSTAGE_VERTEX

    // Recursos opcionais de bindings (ex: uniform buffers, samplers)
    // Se o vertex shader não usa uniforms globais, mantenha em 0
    shaderInfo.num_samplers = 0;
    shaderInfo.num_storage_textures = 0;
    shaderInfo.num_storage_buffers = 0;
    shaderInfo.num_uniform_buffers = 0;

    // 3. Criar e retornar o objeto de shader da GPU
    SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
    if (!shader) {
        SDL_Log("Falha ao criar o objeto do shader na GPU: %s", SDL_GetError());
    }

    return shader;
}

//---
#version 450

// Atributos de Entrada (Enviados pelo VBO do C++)
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

// Atributos de Saída (Enviados para o Fragment Shader)
layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragUV;

void main() {
    // Transmite a posição do vértice
    gl_Position = vec4(inPos, 1.0);

    // Passa os dados adiante para serem interpolados por pixel
    fragColor = inColor;
    fragNormal = inNormal;
    fragUV = inUV;
}

//----
#version 450

// Atributos de Entrada (Recebidos do Vertex Shader - devem bater em location e tipo)
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragUV;

// Atributo de Saída (O pixel final que será desenhado na tela/render target)
layout(location = 0) out vec4 outColor;

void main() {
    // Exemplo básico: Combinando a cor do vértice com a normal para um visual de debug
    vec3 lighting = max(dot(normalize(fragNormal), vec3(0.0, 0.0, 1.0)), 0.0) * fragColor;

    // Saída final (RGBA)
    outColor = vec4(lighting, 1.0);
}

//---
// 1. Carregar os dois shaders usando a função LoadSPIRVShader do passo anterior
SDL_GPUShader* vertexShader = LoadSPIRVShader(device, "shader.vert.spv", SDL_GPU_SHADERSTAGE_VERTEX);
SDL_GPUShader* fragmentShader = LoadSPIRVShader(device, "shader.frag.spv", SDL_GPU_SHADERSTAGE_FRAGMENT);

// 2. Configurar a descrição dos Atributos de Vértice (Exatamente como definido no vert)
SDL_GPUVertexAttribute vertexAttributes[4];

// Posição (location = 0)
vertexAttributes[0].location = 0;
vertexAttributes[0].buffer_slot = 0;
vertexAttributes[0].format = SDL_GPU_VERTEXFORMAT_FLOAT3;
vertexAttributes[0].offset = offsetof(Vertex, pos);

// Cor (location = 1)
vertexAttributes[1].location = 1;
vertexAttributes[1].buffer_slot = 0;
vertexAttributes[1].format = SDL_GPU_VERTEXFORMAT_FLOAT3;
vertexAttributes[1].offset = offsetof(Vertex, color);

// Normal (location = 2)
vertexAttributes[2].location = 2;
vertexAttributes[2].buffer_slot = 0;
vertexAttributes[2].format = SDL_GPU_VERTEXFORMAT_FLOAT3;
vertexAttributes[2].offset = offsetof(Vertex, normal);

// UV (location = 3)
vertexAttributes[3].location = 3;
vertexAttributes[3].buffer_slot = 0;
vertexAttributes[3].format = SDL_GPU_VERTEXFORMAT_FLOAT2;
vertexAttributes[3].offset = offsetof(Vertex, uv);

// Descrição do buffer de entrada
SDL_GPUVertexBufferDescription bufferDesc{};
bufferDesc.slot = 0;
bufferDesc.pitch = sizeof(Vertex);
bufferDesc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;

// 3. Montar a Pipeline de Gráficos
SDL_GPUGraphicsPipelineCreateInfo pipelineInfo{};
pipelineInfo.vertex_shader = vertexShader;
pipelineInfo.fragment_shader = fragmentShader;

// Vincular as definições de vértices configuradas acima
pipelineInfo.vertex_input_state.vertex_attributes = vertexAttributes;
pipelineInfo.vertex_input_state.num_vertex_attributes = 4;
pipelineInfo.vertex_input_state.vertex_buffer_descriptions = &bufferDesc;
pipelineInfo.vertex_input_state.num_vertex_buffer_descriptions = 1;

// Configurações básicas de topologia (Triângulos)
pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

// Configurar o formato de saída do render target (Ex: combinando com o formato da janela)
SDL_GPUColorTargetDescription colorTarget{};
colorTarget.format = SDL_GetGPUSwapchainTextureFormat(device, window);
pipelineInfo.target_info.color_target_descriptions = &colorTarget;
pipelineInfo.target_info.num_color_targets = 1;

// Criar a pipeline final
SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);

// Os shaders individuais podem ser liberados após a criação da pipeline se você não for reutilizá-los
SDL_ReleaseGPUShader(device, vertexShader);
SDL_ReleaseGPUShader(device, fragmentShader);

//---
#version 450

// Atributos de Vértice
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

// Saídas para o Fragment Shader
layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragUV;

// Uniform Buffer para a matriz MVP (Obrigatório set=1 para Vertex Shader na SDL_GPU)
layout(set = 1, binding = 0) confidential uniform UBO { mat4 mvp; }
ubo;

void main() {
    // Aplica a transformação MVP na posição do vértice
    gl_Position = ubo.mvp * vec4(inPos, 1.0);

    fragColor = inColor;
    fragNormal = inNormal;
    fragUV = inUV;
}

//--
#version 450

// Entradas do Vertex Shader
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

// Textura e Sampler combinados (Obrigatório set=2 para Fragment Shader na SDL_GPU)
layout(set = 2, binding = 0) uniform sampler2D texSampler;

void main() {
    // Amostra a cor da textura usando as coordenadas UV
    vec4 texColor = texture(texSampler, fragUV);

    // Multiplica a cor da textura pela cor do vértice para modulação
    outColor = texColor * vec4(fragColor, 1.0);
}

//--
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Estrutura que espelha exatamente o UBO do Vertex Shader
struct UniformBufferObject {
    glm::mat4 mvp;
};

// --- PASSO 1: ATUALIZAR A CRIAÇÃO DOS SHADERS ---

// Ao carregar o Vertex Shader, informe o Uniform Buffer
SDL_GPUShaderCreateInfo vertInfo{};
// (... carregar código SPIR-V no vertInfo.code ...)
vertInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
vertInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
vertInfo.num_uniform_buffers = 1; // <--- Declarar 1 UBO
SDL_GPUShader* vertexShader = SDL_CreateGPUShader(device, &vertInfo);

// Ao carregar o Fragment Shader, informe o Sampler
SDL_GPUShaderCreateInfo fragInfo{};
// (... carregar código SPIR-V no fragInfo.code ...)
fragInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
fragInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
fragInfo.num_samplers = 1; // <--- Declarar 1 Sampler
SDL_GPUShader* fragmentShader = SDL_CreateGPUShader(device, &fragInfo);

// --- PASSO 2: CRIAR RECURSOS DE TEXTURA E SAMPLER ---

// Criar Sampler (define como a textura filtra e repete)
SDL_GPUSamplerCreateInfo samplerInfo{};
samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
SDL_GPUSampler* textureSampler = SDL_CreateGPUSampler(device, &samplerInfo);

// Criar a Textura (Exemplo: 512x512 RGBA)
SDL_GPUTextureCreateInfo textureInfo{};
textureInfo.type = SDL_GPU_TEXTURETYPE_2D;
textureInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
textureInfo.width = 512;
textureInfo.height = 512;
textureInfo.layer_count_or_depth = 1;
textureInfo.num_levels = 1;
textureInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER; // Pronta para ser lida no shader
SDL_GPUTexture* texture = SDL_CreateGPUTexture(device, &textureInfo);

/*
   Nota: Para enviar pixels para a 'texture', você usaria um SDL_GPUTransferBuffer
   e a função SDL_UploadToGPUTexture de forma análoga ao que fizemos com o VBO.
*/

// --- PASSO 3: RENDER LOOP (VÍNCULO DE MATRIZES E TEXTURAS) ---

void RenderFrame(SDL_GPUDevice* device, SDL_GPURenderPass* renderPass, SDL_GPUGraphicsPipeline* pipeline) {

    // Calculando a matriz MVP com GLM
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), SDL_GetTicks() * 0.001f, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(glm::Milford(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);

    // Correção do Vulkan: O eixo Y da projeção do GLM é invertido em relação ao Vulkan
    proj[1][1] *= -1;

    UniformBufferObject ubo{proj * view * model};

    // 1. Vincular a Pipeline
    SDL_BindGPUGraphicsPipeline(renderPass, pipeline);

    // 2. Enviar dados de Uniform de forma dinâmica (Push Uniforms)
    // O Slot 0 corresponde ao 'binding = 0' do shader.
    SDL_PushGPUVertexUniformData(renderPass, 0, &ubo, sizeof(UniformBufferObject));

    // 3. Vincular a Textura e o Sampler no Fragment Shader
    // O Slot 0 corresponde ao 'binding = 0' do shader.
    SDL_GPUTextureSamplerBinding textureBinding{texture, textureSampler};
    SDL_BindGPUFragmentSamplers(renderPass, 0, &textureBinding, 1);

    // 4. Vincular VBO, IBO e Desenhar
    // (... códigos de SDL_BindGPUVertexBuffers e SDL_BindGPUIndexBuffer do passo anterior ...)
    // SDL_DrawGPUIndexedPrimitives(renderPass, indices.size(), 1, 0, 0, 0);
}

//--
Para integrar texturas 2D e matrizes MVP utilizando SDL_GPU com Vulkan,
    precisamos ajustar os shaders e atualizar o código em C++.No ecossistema SDL_GPU para Vulkan / SPIR - V,
    existem regras rígidas de binding
    : os Uniform Buffers do Vertex Shader devem usar obrigatoriamente
          set = 1,
          binding = 0, enquanto texturas e samplers no Fragment Shader utilizam set = 2,
          binding = 0. Abaixo está a implementação completa dessa
                        estrutura.-- -- -- -- -- -- -- -- -- -- -- -- -- -- --##1. Código dos Shaders(
                                     GLSL)##shader.vert(Com suporte a matriz MVP)

#version 450
                    // Atributos de Vérticelayout(location = 0) in vec3 inPos;layout(location = 1) in vec3
                    // inColor;layout(location = 2) in vec3 inNormal;layout(location = 3) in vec2 inUV; Saídas para o
                    // Fragment Shaderlayout(location = 0) out vec3 fragColor;layout(location = 1) out vec3
                    // fragNormal;layout(location = 2) out vec2 fragUV; Uniform Buffer para a matriz MVP (Obrigatório
                    // set=1 para Vertex Shader na SDL_GPU)layout(set = 1, binding = 0) confidential uniform UBO {
                    mat4 mvp;
}
ubo;
void main() {
    // Aplica a transformação MVP na posição do vértice
    gl_Position = ubo.mvp * vec4(inPos, 1.0);

    fragColor = inColor;
    fragNormal = inNormal;
    fragUV = inUV;
}

##shader.frag(Com suporte a Textura / Sampler)

#version 450
    // Entradas do Vertex Shaderlayout(location = 0) in vec3 fragColor;layout(location = 1) in vec3
    // fragNormal;layout(location = 2) in vec2 fragUV;
    layout(location = 0) out vec4 outColor;
// Textura e Sampler combinados (Obrigatório set=2 para Fragment Shader na SDL_GPU)layout(set = 2, binding = 0) uniform
// sampler2D texSampler;
void main() {
    // Amostra a cor da textura usando as coordenadas UV
    vec4 texColor = texture(texSampler, fragUV);

    // Multiplica a cor da textura pela cor do vértice para modulação
    outColor = texColor * vec4(fragColor, 1.0);
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- --##2. Configuração do Código
                                              C++(SDL3)Para enviar esses novos recursos à pipeline,
    você precisará atualizar a criação dos shaders informando que eles utilizam recursos
    extras(1 Uniform Buffer e 1 Sampler),
    além de empacotar a matriz do GLM corretamente.

#include <SDL3/SDL.h>#include <glm/glm.hpp>#include <glm/gtc/matrix_transform.hpp>
    // Estrutura que espelha exatamente o UBO do Vertex Shaderstruct UniformBufferObject {
    glm::mat4 mvp;
}
;
// --- PASSO 1: ATUALIZAR A CRIAÇÃO DOS SHADERS ---
// Ao carregar o Vertex Shader, informe o Uniform BufferSDL_GPUShaderCreateInfo vertInfo{};// (... carregar código
// SPIR-V no vertInfo.code ...)
vertInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
vertInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
vertInfo.num_uniform_buffers =
    1; // <--- Declarar 1 UBOSDL_GPUShader* vertexShader = SDL_CreateGPUShader(device, &vertInfo);
// Ao carregar o Fragment Shader, informe o SamplerSDL_GPUShaderCreateInfo fragInfo{};// (... carregar código SPIR-V no
// fragInfo.code ...)
fragInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
fragInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
fragInfo.num_samplers =
    1; // <--- Declarar 1 SamplerSDL_GPUShader* fragmentShader = SDL_CreateGPUShader(device, &fragInfo);

// --- PASSO 2: CRIAR RECURSOS DE TEXTURA E SAMPLER ---
// Criar Sampler (define como a textura filtra e repete)SDL_GPUSamplerCreateInfo samplerInfo{};
samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
SDL_GPUSampler* textureSampler = SDL_CreateGPUSampler(device, &samplerInfo);
// Criar a Textura (Exemplo: 512x512 RGBA)SDL_GPUTextureCreateInfo textureInfo{};
textureInfo.type = SDL_GPU_TEXTURETYPE_2D;
textureInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
textureInfo.width = 512;
textureInfo.height = 512;
textureInfo.layer_count_or_depth = 1;
textureInfo.num_levels = 1;
textureInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER; // Pronta para ser lida no shaderSDL_GPUTexture* texture =
                                                  // SDL_CreateGPUTexture(device, &textureInfo);
/*
   Nota: Para enviar pixels para a 'texture', você usaria um SDL_GPUTransferBuffer
   e a função SDL_UploadToGPUTexture de forma análoga ao que fizemos com o VBO.
*/

// --- PASSO 3: RENDER LOOP (VÍNCULO DE MATRIZES E TEXTURAS) ---
void RenderFrame(SDL_GPUDevice* device, SDL_GPURenderPass* renderPass, SDL_GPUGraphicsPipeline* pipeline) {

    // Calculando a matriz MVP com GLM
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), SDL_GetTicks() * 0.001f, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(glm::Milford(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);

    // Correção do Vulkan: O eixo Y da projeção do GLM é invertido em relação ao Vulkan
    proj[1][1] *= -1;

    UniformBufferObject ubo{proj * view * model};

    // 1. Vincular a Pipeline
    SDL_BindGPUGraphicsPipeline(renderPass, pipeline);

    // 2. Enviar dados de Uniform de forma dinâmica (Push Uniforms)
    // O Slot 0 corresponde ao 'binding = 0' do shader.
    SDL_PushGPUVertexUniformData(renderPass, 0, &ubo, sizeof(UniformBufferObject));

    // 3. Vincular a Textura e o Sampler no Fragment Shader
    // O Slot 0 corresponde ao 'binding = 0' do shader.
    SDL_GPUTextureSamplerBinding textureBinding{texture, textureSampler};
    SDL_BindGPUFragmentSamplers(renderPass, 0, &textureBinding, 1);

    // 4. Vincular VBO, IBO e Desenhar
    // (... códigos de SDL_BindGPUVertexBuffers e SDL_BindGPUIndexBuffer do passo anterior ...)
    // SDL_DrawGPUIndexedPrimitives(renderPass, indices.size(), 1, 0, 0, 0);
}

##O que mudou aqui ?

                   *SDL_PushGPUVertexUniformData
                   : A SDL_GPU possui uma forma altamente eficiente de enviar dados uniformes pequenos que mudam todo
                     frame(como matrizes de transformação)
                         .Em vez de alocar um buffer estático gerenciado manualmente,
    você faz o "push" dos dados diretamente no passo de renderização(RenderPass)
        .*Inversão do Eixo Y(proj[1][1] *= -1)
    : O GLM foi originalmente desenhado para OpenGL(onde o topo da tela é Y = 1)
        .No Vulkan,
o topo da tela é Y =
    -1. Multiplicar essa posição da matriz de projeção corrige instantaneamente objetos que apareçam de ponta
    - cabeça na tela.*
          SDL_GPUTextureSamplerBinding
    : Passamos tanto o ponteiro da textura quanto as regras de
      amostragem(filtros linear / focado) agrupados em uma única chamada que alimenta o sampler2D do GLSL.

      Se você precisar de ajuda para carregar texturas do disco via arquivos PNG
          /
          JPG usando a biblioteca
          SDL_image(compatível com a SDL3) e fazer o upload desses pixels para o objeto SDL_GPUTexture,
                 me avise para detalharmos essa etapa !
