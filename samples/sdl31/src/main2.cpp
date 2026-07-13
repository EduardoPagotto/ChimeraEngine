#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <fstream>
#include <iostream>
#include <vector>

// Função auxiliar para carregar o arquivo binário SPIR-V
SDL_GPUShader* LoadShader(SDL_GPUDevice* device, const std::string& filename, SDL_GPUShaderStage stage,
                          Uint32 numSamplers) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Falha ao abrir shader: " << filename << std::endl;
        return nullptr;
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    SDL_GPUShaderCreateInfo shaderInfo{};
    shaderInfo.code_size = fileSize;
    shaderInfo.code = reinterpret_cast<const Uint8*>(buffer.data());
    shaderInfo.entrypoint = "main";
    shaderInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
    shaderInfo.stage = stage;
    shaderInfo.num_samplers = numSamplers; // 1 para o fragment shader (texSampler)

    return SDL_CreateGPUShader(device, &shaderInfo);
}

int main(int argc, char* argv[]) {
    // 1. Inicializar o SDL e o subsistema de GPU
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL3 Vulkan Texture", 800, 600, SDL_WINDOW_VULKAN);
    if (!window) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Força o SDL_GPU a usar o driver do Vulkan
    SDL_SetHint(SDL_HINT_GPU_DRIVER, "vulkan");

    SDL_GPUDevice* device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
    if (!device) {
        std::cerr << "Erro ao criar GPU Device (Vulkan): " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (!SDL_ClaimWindowForGPUDevice(device, window)) {
        std::cerr << "Erro ao associar janela ao dispositivo: " << SDL_GetError() << std::endl;
        SDL_DestroyGPUDevice(device);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // 2. Carregar a imagem do arquivo para a CPU (RAM)
    // Nota: Certifique-se de que a imagem esteja em formato RGBA de 32 bits
    SDL_Surface* surface = IMG_Load("./assets/textures/grid2.png");
    if (!surface) {
        std::cerr << "Erro ao carregar imagem: " << SDL_GetError() << std::endl;
        SDL_DestroyGPUDevice(device);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Converter para RGBA32 se necessário para garantir compatibilidade com a GPU
    SDL_Surface* rgbaSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);

    // 3. Criar a textura na GPU (VRAM)
    SDL_GPUTextureCreateInfo textureInfo{};
    textureInfo.type = SDL_GPU_TEXTURETYPE_2D;
    textureInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    textureInfo.width = rgbaSurface->w;
    textureInfo.height = rgbaSurface->h;
    textureInfo.layer_count_or_depth = 1;
    textureInfo.num_levels = 1;
    textureInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER; // Usada em shaders

    SDL_GPUTexture* gpuTexture = SDL_CreateGPUTexture(device, &textureInfo);

    // 4. Criar o Transfer Buffer (Staging Buffer) para enviar dados à GPU
    Uint32 imageSize = rgbaSurface->pitch * rgbaSurface->h;

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transferInfo.size = imageSize;

    SDL_GPUTransferBuffer* stagingBuffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    // Mapear o buffer, copiar os pixels da CPU e desmapear
    void* bufferData = SDL_MapGPUTransferBuffer(device, stagingBuffer, false);
    memcpy(bufferData, rgbaSurface->pixels, imageSize);
    SDL_UnmapGPUTransferBuffer(device, stagingBuffer);

    // Liberar a superfície da CPU pois os dados já estão no staging buffer
    SDL_DestroySurface(rgbaSurface);

    // 5. Enviar os dados do Staging Buffer para a Textura da GPU via Command Buffer
    SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

    SDL_GPUTextureTransferInfo sourceInfo{};
    sourceInfo.transfer_buffer = stagingBuffer;
    sourceInfo.offset = 0; // Início do buffer

    SDL_GPUTextureRegion destRegion{};
    destRegion.texture = gpuTexture;
    destRegion.w = textureInfo.width;
    destRegion.h = textureInfo.height;
    destRegion.d = 1;

    SDL_UploadToGPUTexture(copyPass, &sourceInfo, &destRegion, false);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmdBuffer); // Executa a transferência no Vulkan

    // O staging buffer pode ser destruído após o envio dos comandos
    SDL_ReleaseGPUTransferBuffer(device, stagingBuffer);

    // 1. Criar o Sampler
    SDL_GPUSamplerCreateInfo samplerInfo{};
    samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
    samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
    samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
    samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

    SDL_GPUSampler* gpuSampler = SDL_CreateGPUSampler(device, &samplerInfo);
    if (!gpuSampler) {
        std::cerr << "Erro ao criar Sampler: " << SDL_GetError() << std::endl;
    }

    // 2. Carregar os Shaders compilados (.spv)
    SDL_GPUShader* vertexShader = LoadShader(device, "vertex.spv", SDL_GPU_SHADERSTAGE_VERTEX, 0);
    SDL_GPUShader* fragmentShader = LoadShader(device, "fragment.spv", SDL_GPU_SHADERSTAGE_FRAGMENT, 1);

    if (!vertexShader || !fragmentShader) {
        std::cerr << "Erro ao carregar shaders SPIR-V." << std::endl;
        return -1;
    }

    // 3. Configurar e Criar a Graphics Pipeline
    SDL_GPUGraphicsPipelineCreateInfo pipelineInfo{};

    // Configura os shaders
    pipelineInfo.vertex_shader = vertexShader;
    pipelineInfo.fragment_shader = fragmentShader;
    pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

    // Configura o formato do anexo de cor (deve bater com o formato da janela)
    SDL_GPUColorTargetDescription colorTarget{};
    colorTarget.format = SDL_GetGPUSwapchainTextureFormat(device, window);
    // Configuração de transparência desativada (substituir a cor antiga)
    colorTarget.blend_state.enable_blend = false;

    pipelineInfo.target_info.color_target_descriptions = &colorTarget;
    pipelineInfo.target_info.num_color_targets = 1;

    SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
    if (!pipeline) {
        std::cerr << "Erro ao criar Graphics Pipeline: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Os módulos de shader originais podem ser liberados após a criação da pipeline
    SDL_ReleaseGPUShader(device, vertexShader);
    SDL_ReleaseGPUShader(device, fragmentShader);

    // 4. Loop Principal de Renderização
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Adquirir um Command Buffer para o frame atual
        SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
        if (!cmdBuffer)
            continue;

        // Adquirir a textura de Swapchain da janela onde desenharemos
        SDL_GPUTexture* swapchainTexture;
        Uint32 windowWidth, windowHeight;
        if (!SDL_AcquireGPUSwapchainTexture(cmdBuffer, window, &swapchainTexture, &windowWidth, &windowHeight)) {
            std::cerr << "Erro ao adquirir Swapchain Texture: " << SDL_GetError() << std::endl;
            SDL_SubmitGPUCommandBuffer(cmdBuffer);
            continue;
        }

        if (swapchainTexture != nullptr) {
            // Iniciar o passe de renderização apontando para a tela
            SDL_GPUColorTargetInfo colorTargetInfo{};
            colorTargetInfo.texture = swapchainTexture;
            colorTargetInfo.clear_color = SDL_FColor{0.1f, 0.2f, 0.3f, 1.0f}; // Fundo azul escuro
            colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
            colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

            SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdBuffer, &colorTargetInfo, 1, nullptr);

            // Ativar a Pipeline de renderização
            SDL_BindGPUGraphicsPipeline(renderPass, pipeline);

            // Vincular a Textura e o Sampler no Slot 0 (bater com layout(binding=0) do Fragment Shader)
            SDL_GPUTextureSamplerBinding binding{};
            binding.texture = gpuTexture;
            binding.sampler = gpuSampler;
            SDL_BindGPUFragmentSamplers(renderPass, 0, &binding, 1);

            // Desenhar 6 vértices (2 triângulos que formam o quadrado na tela)
            SDL_DrawGPUPrimitives(renderPass, 6, 1, 0, 0);

            SDL_EndGPURenderPass(renderPass);
        }

        // Enviar os comandos criados para a execução física na GPU (Vulkan)
        SDL_SubmitGPUCommandBuffer(cmdBuffer);
    }

    // 5. Limpeza de Recursos adicionais
    SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
    SDL_ReleaseGPUSampler(device, gpuSampler);

    // [Restante da limpeza do código anterior: textura, window, device, quit...]
    return 0;
}

// // --- Loop principal simplificado ---
// bool running = true;
// while (running) {
//     SDL_Event event;
//     while (SDL_PollEvent(&event)) {
//         if (event.type == SDL_EVENT_QUIT) {
//             running = false;
//         }
//     }

//     // Renderização da textura aconteceria aqui utilizando pipelines e samplers...
// }

// // Limpeza de recursos
// SDL_ReleaseGPUTexture(device, gpuTexture);
// SDL_ReleaseWindowFromGPUDevice(device, window);
// SDL_DestroyGPUDevice(device);
// SDL_DestroyWindow(window);
// SDL_Quit();
//
// return 0;
//}
