#pragma once

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <cstddef>
#include <format>
#include <stdexcept>

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
  private:
};
