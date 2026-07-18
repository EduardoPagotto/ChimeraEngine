#include <SDL3/SDL.h>
#include <fstream>
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

void attr() {
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
}

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
    vertexAttributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    vertexAttributes[0].offset = offsetof(Vertex, pos);

    // Cor (glm::vec3)
    vertexAttributes[1].location = 1;
    vertexAttributes[1].buffer_slot = 0;
    vertexAttributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    vertexAttributes[1].offset = offsetof(Vertex, color);

    // Normal (glm::vec3)
    vertexAttributes[2].location = 2;
    vertexAttributes[2].buffer_slot = 0;
    vertexAttributes[2].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    vertexAttributes[2].offset = offsetof(Vertex, normal);

    // UV (glm::vec2)
    vertexAttributes[3].location = 3;
    vertexAttributes[3].buffer_slot = 0;
    vertexAttributes[3].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
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
