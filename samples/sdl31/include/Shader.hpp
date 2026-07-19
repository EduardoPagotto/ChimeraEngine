#pragma once

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <cstddef>
#include <format>
#include <stdexcept>
#include <vector>

class GPU {
  public:
    explicit GPU() = default;
    explicit GPU(const std::string& title, int widht, int heigh) { this->create(title, widht, heigh); }

    GPU(const GPU&) = delete;
    GPU& operator=(const GPU&) = delete;

    void create(const std::string& title, int widht, int heigh) {

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error(std::format("Não foi possível inicializar o SDL: {}", SDL_GetError()));
        }

        this->window = SDL_CreateWindow(title.c_str(), widht, heigh, SDL_WINDOW_RESIZABLE);
        if (window == nullptr) {
            throw std::runtime_error(std::format("Erro ao criar janela: {}", SDL_GetError()));
        }

        this->device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);
        if (!device || !SDL_ClaimWindowForGPUDevice(this->device, this->window)) {
            throw std::runtime_error(std::format("Erro ao inicializar GPU/Janela: {}", SDL_GetError()));
        }

        // Obtém o nome do driver de backend (deve retornar "vulkan")
        const char* backend_name = SDL_GetGPUDeviceDriver(this->device);
        SDL_Log("Backend ativo: %s", backend_name);

        SDL_PropertiesID props = SDL_GetGPUDeviceProperties(this->device);
        const char* device_name = SDL_GetStringProperty(props, "SDL.gpu.device.name", "Desconhecido");
        SDL_Log("Dispositivo físico detectado: %s", device_name);
    }

    void destroy() {
        if ((this->device != nullptr) && (this->window != nullptr)) {
            SDL_ReleaseWindowFromGPUDevice(this->device, this->window);
            SDL_DestroyGPUDevice(this->device);
            SDL_DestroyWindow(this->window);
            this->device = nullptr;
            this->window = nullptr;
        }
    }

    virtual ~GPU() noexcept { this->destroy(); }

    SDL_GPUDevice* getDevice() { return this->device; }
    SDL_Window* getWindow() { return this->window; }

  private:
    SDL_GPUDevice* device{nullptr};
    SDL_Window* window{nullptr};
};

class Shader {
  public:
    explicit Shader() = default;
    explicit Shader(SDL_GPUDevice* device, const char* filename, Uint32 samplerCount) {
        this->create(device, filename, samplerCount);
    }

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    virtual ~Shader() noexcept { this->destroy(); }

    void create(SDL_GPUDevice* device, const char* filename, Uint32 samplerCount) {
        this->device = device;
        size_t size;
        void* code = SDL_LoadFile(filename, &size);
        if (!code) {
            throw std::runtime_error(std::format("Erro ao ler arquivo de shader: {}", filename));
        }

        const SDL_GPUShaderCreateInfo shaderInfo = {
            .code_size = size,
            .code = static_cast<const Uint8*>(code),
            .entrypoint = "main",
            .format = SDL_GPU_SHADERFORMAT_SPIRV, // Modifique conforme seu backend (ex: DXBC, MSL)
            .stage = (samplerCount > 0) ? SDL_GPU_SHADERSTAGE_FRAGMENT : SDL_GPU_SHADERSTAGE_VERTEX,
            .num_samplers = samplerCount};

        this->handler = SDL_CreateGPUShader(device, &shaderInfo);

        if (this->handler == nullptr) {
            throw std::runtime_error(std::format("Falha ao criar o shader {}", filename));
        }

        SDL_free(code);
    }

    void destroy() {
        if ((this->device != nullptr) && (this->handler != nullptr)) {
            SDL_ReleaseGPUShader(this->device, this->handler);
            this->handler = nullptr;
        }
    }

    SDL_GPUShader* get() { return this->handler; }

  private:
    SDL_GPUDevice* device{nullptr};
    SDL_GPUShader* handler{nullptr};
};

class Buffer {
  public:
    explicit Buffer(SDL_GPUDevice* device) : device(device) {}

    void create(const SDL_GPUBufferUsageFlags& usage, uint32_t size) {

        this->size = size;
        const SDL_GPUBufferCreateInfo vboInfo{
            .usage = usage, // SDL_GPU_BUFFERUSAGE_VERTEX;
            .size = size    // vertices.size() * sizeof(Vertex);
        };

        this->buffer = SDL_CreateGPUBuffer(this->device, &vboInfo);

        if (this->buffer == nullptr) {
            throw std::runtime_error(std::format("Fail to create buffer"));
        }
    }

    virtual ~Buffer() noexcept { this->destroy(); }

    SDL_GPUBuffer* get() const { return this->buffer; }

    void destroy() {
        if (buffer != nullptr) {
            SDL_ReleaseGPUBuffer(this->device, this->buffer);
            this->buffer = nullptr;
        }
    }

    const uint32_t getSize() const { return this->size; }

  private:
    uint32_t size;
    SDL_GPUDevice* device{nullptr};
    SDL_GPUBuffer* buffer{nullptr};
};

// void copy_to_gpu(SDL_GPUDevice* device, std::vector<Buffer> vBuffers) {

//     SDL_GPUTransferBufferCreateInfo transferInfo{};
//     transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;

//     uint32_t tot = 0;
//     for (auto& buffer : vBuffers) {
//         tot += buffer.getSize();
//     }

//     transferInfo.size = tot;
//     SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

//     // Mapear e copiar os dados para o buffer de transferência
//     uint8_t* mapPtr = (uint8_t*)SDL_MapGPUTransferBuffer(device, transferBuffer, false);

//     uint32_t acc = 0;
//     for (auto& buffer : vBuffers) {
//         //
//         SDL_memcpy(mapPtr + acc, buffer.data(), buffer.size);
//     }

//     SDL_memcpy(mapPtr, vertices.data(), vboInfo.size);

//     SDL_memcpy(mapPtr + vboInfo.size, indices.data(), iboInfo.size);

//     SDL_UnmapGPUTransferBuffer(device, transferBuffer);
// }
