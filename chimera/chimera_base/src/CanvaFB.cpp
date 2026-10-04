#include "chimera_base/CanvasFB.hpp"
#include <SDL3/SDL_pixels.h>
#include <format>
#include <memory>
#include <stdexcept>

namespace ce {

    CanvaFB::CanvaFB(const std::string& title, int width, int height, bool full_screen)
        : full_screen_(full_screen), pixel_format_(SDL_PIXELFORMAT_RGBA8888), title_(title) {

        if (!SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland")) {
            throw std::runtime_error(std::format("SDL wayland Failed driver: {}", SDL_GetError()));
        }

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("Falha SDL_Init:" + std::string(SDL_GetError()));
        }

        this->window_ = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (this->window_ == nullptr) {
            throw std::runtime_error(std::format("Couldn't create window: {}", SDL_GetError()));
        }

        this->renderer_ = SDL_CreateRenderer(window_, nullptr);
        if (renderer_ == nullptr) {
            throw std::runtime_error(std::format("Couldn't create renderer: {})", SDL_GetError()));
        }

        SDL_SetRenderVSync(renderer_, 1); // Ativa o Vsync no SDL3

        // Necessario pelo wayland
        if (!SDL_RenderPresent(renderer_)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render present: %s", SDL_GetError());
        }

        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_FORMAT_NUMBER, pixel_format_);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_ACCESS_NUMBER, SDL_TEXTUREACCESS_STREAMING);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_WIDTH_NUMBER, width);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_HEIGHT_NUMBER, height);

        // Forçar o espaço de cores correto (sRGB) para evitar o tom desbotado
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_COLORSPACE_NUMBER, SDL_COLORSPACE_SRGB);

        // Criar a textura a partir das propriedades
        this->texture_ = SDL_CreateTextureWithProperties(renderer_, props);
        // this->texture = SDL_CreateTexture(renderer, this->pixelFormat, SDL_TEXTUREACCESS_STREAMING, width, height);
        if (this->texture_ == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "create texture: %s", SDL_GetError());
        }

        SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);

        // Limpar as propriedades da memória após o uso (a textura já foi criada)
        SDL_DestroyProperties(props);

        this->pixel_canvas_ = std::make_shared<PixelCanvas>(width, height, 0, this->pixel_format_);
    }

    void CanvaFB::before() {
        // limpa framebuffer
        this->pixel_canvas_->clear(0);

        //  Limpa a tela atual
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);

        if (!SDL_RenderClear(renderer_)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render clear: %s", SDL_GetError());
        }
    }

    void CanvaFB::after() {

        // TODO: Colocar no CMAKE este def
#define FRAMEBUFFER_GPU_SET 1

#ifdef FRAMEBUFFER_GPU_SET
        // Atualiza a textura pela GPU
        if (!SDL_UpdateTexture(texture_, nullptr, this->pixel_canvas_->getPixelsView().data(),
                               static_cast<int>(this->pixel_canvas_->getWithSize()))) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "update texture: %s", SDL_GetError());
        }
#else
        // Copia pela CPU
        char* pixel{nullptr};
        int pitch{0};
        SDL_LockTexture(texture, nullptr, (void**)&pixel, &pitch);

        for (std::size_t i = 0, sp = 0, dp = 0; i < this->pixelCanvas->getHeight();
             i++, dp += this->pixelCanvas->getWidth(), sp += pitch) {

            std::memcpy(pixel + sp, this->pixelCanvas->getPixelsView().data() + dp, this->pixelCanvas->getWithSize());
        }

        SDL_UnlockTexture(texture);
        // fim copia
#endif
        // Copia a textura
        if (!SDL_RenderTexture(renderer_, texture_, nullptr, nullptr)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render texture: %s", SDL_GetError());
        }

        // exibe
        if (!SDL_RenderPresent(renderer_)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render present: %s", SDL_GetError());
        }
    }

    void CanvaFB::toggleFullScreen() {
        if (!full_screen_) {

            SDL_GetWindowPosition(window_, &position_.x, &position_.y);

            SDL_SetWindowPosition(window_, 0, 0);
            SDL_SetWindowFullscreen(window_, true);

        } else {

            SDL_SetWindowFullscreen(window_, false);
            SDL_SetWindowPosition(window_, position_.x, position_.y);
        }

        full_screen_ = !full_screen_;
    }

    CanvaFB::~CanvaFB() {
        SDL_DestroyTexture(this->texture_);
        SDL_DestroyRenderer(this->renderer_);
        SDL_DestroyWindow(this->window_);

        this->pixel_canvas_.reset();
    }

    void CanvaFB::reshape(int width, int height) {
        this->pixel_canvas_.reset();
        this->pixel_canvas_ = std::make_shared<PixelCanvas>(width, height, 0, this->pixel_format_);
    }

} // namespace ce
