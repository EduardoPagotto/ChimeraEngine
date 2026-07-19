#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Shader.hpp"

GPU gpu;
Shader vertShader;
Shader fragShader;

SDL_GPUTexture* LoadTextureFromFile(SDL_GPUDevice* device, const char* filepath) {
    // 1. Carrega a imagem do disco para uma SDL_Surface na CPU
    SDL_Surface* surface = IMG_Load(filepath);
    if (!surface) {
        SDL_Log("Erro ao carregar a imagem: %s", SDL_GetError());
        return NULL;
    }

    // 2. Garante que os pixels estejam no formato RGBA de 32 bits exigido pela GPU
    SDL_Surface* rgbaSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface); // Libera a superfície original
    if (!rgbaSurface) {
        SDL_Log("Erro ao converter o formato da superfície: %s", SDL_GetError());
        return NULL;
    }

    // 3. Cria a textura final na memória interna da GPU
    SDL_GPUTextureCreateInfo textureInfo = {.type = SDL_GPU_TEXTURETYPE_2D,
                                            .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
                                            .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER, // Usada como textura em shaders
                                            .width = static_cast<Uint32>(rgbaSurface->w),
                                            .height = static_cast<Uint32>(rgbaSurface->h),
                                            .layer_count_or_depth = 1,
                                            .num_levels = 1,
                                            .sample_count = SDL_GPU_SAMPLECOUNT_1};

    SDL_GPUTexture* gpuTexture = SDL_CreateGPUTexture(device, &textureInfo); //
    if (!gpuTexture) {
        SDL_Log("Erro ao criar a textura na GPU: %s", SDL_GetError());
        SDL_DestroySurface(rgbaSurface);
        return NULL;
    }

    // 4. Aloca um Transfer Buffer na CPU/Host para preparar o upload dos dados
    Uint32 imageSize = rgbaSurface->pitch * rgbaSurface->h;
    SDL_GPUTransferBufferCreateInfo transferBufferInfo = {.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
                                                          .size = imageSize};

    SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &transferBufferInfo); //
    if (!transferBuffer) {
        SDL_Log("Erro ao criar o Transfer Buffer: %s", SDL_GetError());
        SDL_ReleaseGPUTexture(device, gpuTexture);
        SDL_DestroySurface(rgbaSurface);
        return NULL;
    }

    // 5. Mapeia a memória do Transfer Buffer para obter um ponteiro acessível pela CPU
    void* mappedMemory = SDL_MapGPUTransferBuffer(device, transferBuffer, false); //
    if (!mappedMemory) {
        SDL_Log("Erro ao mapear o Transfer Buffer: %s", SDL_GetError());
        SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
        SDL_ReleaseGPUTexture(device, gpuTexture);
        SDL_DestroySurface(rgbaSurface);
        return NULL;
    }

    // Copia os dados da superfície (CPU) para a memória mapeada (Transfer Buffer)
    SDL_memcpy(mappedMemory, rgbaSurface->pixels, imageSize);

    // Desmapeia imediatamente após a cópia para permitir que os comandos de upload funcionem
    SDL_UnmapGPUTransferBuffer(device, transferBuffer); //
    SDL_DestroySurface(rgbaSurface);                    // A superfície não é mais necessária

    // 6. Inicia a gravação de comandos da GPU para realizar a transferência interna
    SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

    SDL_GPUTextureTransferInfo sourceInfo = {.transfer_buffer = transferBuffer,
                                             .offset = 0,
                                             .pixels_per_row = textureInfo.width, // Dados compactados linha por linha
                                             .rows_per_layer = textureInfo.height};

    SDL_GPUTextureRegion destRegion = {.texture = gpuTexture, .w = textureInfo.width, .h = textureInfo.height, .d = 1};

    // Executa a cópia do Transfer Buffer para a textura definitiva da GPU
    SDL_UploadToGPUTexture(copyPass, &sourceInfo, &destRegion, false); //

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmdBuffer);

    // 7. Libera o Transfer Buffer provisório (os dados já foram agendados para a GPU)
    SDL_ReleaseGPUTransferBuffer(device, transferBuffer); //

    return gpuTexture;
}

int main(int argc, char* argv[]) {

    gpu.create("Teste z1", 800, 600);

    // Carrega a textura e os shaders
    SDL_GPUTexture* myTexture = LoadTextureFromFile(gpu.getDevice(), "./assets/textures/grid2.png");
    vertShader.create(gpu.getDevice(), "./bin/TexturedQuad.vert.spv", 0);
    fragShader.create(gpu.getDevice(), "./bin/TexturedQuad.frag.spv", 1);

    if (!myTexture) {
        SDL_Log("Falha ao carregar ativos essenciais.");
        return -1;
    }

    // 5. Cria um Amostrador (Sampler) para ditar como ler a textura (filtração bilinear)
    SDL_GPUSamplerCreateInfo samplerInfo = {.min_filter = SDL_GPU_FILTER_LINEAR,
                                            .mag_filter = SDL_GPU_FILTER_LINEAR,
                                            .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR,
                                            .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
                                            .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE};

    SDL_GPUSampler* sampler = SDL_CreateGPUSampler(gpu.getDevice(), &samplerInfo);

    // 6. Define e constrói o Pipeline de Renderização (Graphics Pipeline)
    const SDL_GPUColorTargetBlendState bs = {
        .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
        .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        .color_blend_op = SDL_GPU_BLENDOP_ADD,
        .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
        .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
        .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
        .enable_blend = true, // Ativa transparências alfa (PNG)
    };

    const SDL_GPUColorTargetDescription td = {
        .format = SDL_GetGPUSwapchainTextureFormat(gpu.getDevice(), gpu.getWindow()), .blend_state = bs};

    const SDL_GPUGraphicsPipelineTargetInfo tf{.color_target_descriptions = &td, .num_color_targets = 1};

    SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {
        .vertex_shader = vertShader.get(),
        .fragment_shader = fragShader.get(),
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLESTRIP, // Desenha o quadrado nativamente sem buffers de índice
        .target_info = tf};

    SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(gpu.getDevice(), &pipelineInfo);

    // Liberar os shaders imediatamente após construir o pipeline diminui o consumo
    vertShader.destroy();
    fragShader.destroy();

    // 7. Loop de Eventos e Renderização Principal
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Adquire um buffer de comando livre para este frame
        SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(gpu.getDevice());
        if (cmdBuffer) {
            SDL_GPUTexture* swapchainTexture;
            Uint32 windowWidth, windowHeight;

            // Espera a textura de framebuffer disponível na tela
            if (SDL_AcquireGPUSwapchainTexture(cmdBuffer, gpu.getWindow(), &swapchainTexture, &windowWidth,
                                               &windowHeight)) {
                SDL_GPUColorTargetInfo colorTarget = {
                    .texture = swapchainTexture,
                    .clear_color = {0.1f, 0.1f, 0.1f, 1.0f}, // Cor de fundo caso a textura falhe
                    .load_op = SDL_GPU_LOADOP_CLEAR,
                    .store_op = SDL_GPU_STOREOP_STORE};

                // Inicia o passo de desenho em tela
                SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdBuffer, &colorTarget, 1, NULL);

                // Vincula os estados imutáveis do pipeline gráfico
                SDL_BindGPUGraphicsPipeline(renderPass, pipeline);

                // Vincula nossa Textura e o Amostrador no índice zero do fragment shader
                SDL_GPUTextureSamplerBinding textureBinding = {.texture = myTexture, .sampler = sampler};
                SDL_BindGPUFragmentSamplers(renderPass, 0, &textureBinding, 1);

                // Emite o comando de desenho para gerar 4 vértices usando Triangle Strip
                SDL_DrawGPUPrimitives(renderPass, 4, 1, 0, 0);

                SDL_EndGPURenderPass(renderPass);
            }
            // Envia todos os comandos agendados de volta para a GPU processar
            SDL_SubmitGPUCommandBuffer(cmdBuffer);
        }
    }

    // 8. Limpeza de Memória na Saída
    SDL_ReleaseGPUGraphicsPipeline(gpu.getDevice(), pipeline);
    SDL_ReleaseGPUSampler(gpu.getDevice(), sampler);
    SDL_ReleaseGPUTexture(gpu.getDevice(), myTexture);

    gpu.destroy();

    SDL_Quit();
    return 0;
}
