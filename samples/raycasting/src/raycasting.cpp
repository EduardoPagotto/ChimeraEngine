#include "raycasting.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <cmath>
#include <format>
#include <stdexcept>

bool LoadWorld(const char filename[], World* world) {
    FILE* file;
    char string[1024];

    file = fopen(filename, "rb");
    if (file == nullptr) {
        return false;
    }

    // tamanho do mapa
    fgets(string, 1024, file);
    world->width = atoi(string);

    fgets(string, 1024, file);
    world->height = atoi(string);

    // alocação de mapa // new uint8_t[world->width * world->height];
    world->data = std::vector<uint8_t>(static_cast<size_t>(world->width) * world->height, 0);

    // carregando mapa
    for (uint32_t h = 0; h < world->height; h++) {
        fgets(string, 1024, file);

        for (uint32_t w = 0; w < world->width; w++) {

            std::size_t indice = w + (h * world->width);

            if (string[w] == 0x20) {
                // Bloco Vazio
                world->data[indice] = 0;
            } else {
                // parede (conversão ASCII completa)
                uint8_t val = (uint8_t)(string[w] - 0x30);
                world->data[indice] = val;
            }
        }
    }

    return true;
}

void DrawColumn(RayHit what, World world, std::shared_ptr<ce::PixelCanvas> pixel_canvas, uint32_t column) {
    // tipo de bloco detectado

    auto pos = what.map.x + what.map.y * world.width;
    if (pos > world.data.size()) {
        throw std::runtime_error(std::format("Poiscao do mapa invalida: {}", pos));
    }

    uint8_t type = world.data[pos];

    const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(pixel_canvas->pixel_format());

    // selecione cor com base no tipo de bloco
    uint32_t cor_val = 0;

    switch (type) {
        case 1:
            cor_val = SDL_MapRGBA(details, nullptr, 0, 255, 0, 255); // green
            break;
        case 2:
            cor_val = SDL_MapRGBA(details, nullptr, 155, 155, 155, 255); // gray
            break;
        case 3:
            cor_val = SDL_MapRGBA(details, nullptr, 0, 0, 255, 255); // blue
            break;
        case 4:
            cor_val = SDL_MapRGBA(details, nullptr, 255, 0, 0, 255); // red
            break;
        default:
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Pixel incompativel");
            break;
    }

    // calcular a altura da coluna
    uint32_t colh = abs(int(pixel_canvas->height() / what.distance));
    uint32_t cropup = 0;
    uint32_t cropdown = 0;
    uint32_t index = 0;

    if (colh > pixel_canvas->height()) // se for maior que a tela, corte
    {
        index = column;
        cropup = (colh - pixel_canvas->height()) / 2;
        cropdown = cropup + 1;
    } else {
        index = column + (((pixel_canvas->height() - colh) / 2) * pixel_canvas->width());
        cropup = 0;
        cropdown = 0;
    }

    // desenhar coluna
    for (uint32_t c = cropup; c < (colh - cropdown); c++) {
        // desenhe o pixel da cor selecionada
        pixel_canvas->pixels()[index] = cor_val;
        index += pixel_canvas->width();
    }
}

void RenderScene(State state, World world, std::shared_ptr<ce::PixelCanvas> pixel_canvas) {

    for (uint32_t column = 0; column < pixel_canvas->width(); column++) // Para cada coluna
    {
        // calcular a posição e direção do feixe
        float camera_x = 2 * column / float(pixel_canvas->width()) - 1;
        glm::vec2 ray_pos = state.pos;
        glm::vec2 ray_dir = state.dir + state.cam * camera_x;

        // o bloco atual onde estamos
        glm::ivec2 map = ray_pos;

        // comprimento do feixe da posição atual para o próximo bloco
        glm::vec2 side_dist;

        // comprimento do raio de um bloco para outro
        glm::vec2 delta_dist(sqrt(1 + (ray_dir.y * ray_dir.y) / (ray_dir.x * ray_dir.x)),
                             sqrt(1 + (ray_dir.x * ray_dir.x) / (ray_dir.y * ray_dir.y)));

        // direção para onde ir (+1 ou -1), tanto para X quanto para Y
        glm::ivec2 step(0, 0);
        if (ray_dir.x < 0) {
            step.x = -1;
            side_dist.x = (ray_pos.x - map.x) * delta_dist.x;
        } else {
            step.x = 1;
            side_dist.x = (map.x + 1.0 - ray_pos.x) * delta_dist.x;
        }

        if (ray_dir.y < 0) {
            step.y = -1;
            side_dist.y = (ray_pos.y - map.y) * delta_dist.y;
        } else {
            step.y = 1;
            side_dist.y = (map.y + 1.0 - ray_pos.y) * delta_dist.y;
        }

        // vamos lançar o raio
        int side; // face do cubo encontrado (face Norte-Sul ou face Oeste-Leste)

        auto pos = map.x + map.y * world.width;
        if (pos > world.data.size()) {
            throw std::runtime_error(std::format("Poiscao do mapa invalida: {}", pos));
        }

        while (world.data[pos] == 0) // até nos encontrarmos com uma parede ...
        {
            // vamos para o próximo bloco no mapa
            if (side_dist.x < side_dist.y) {
                side_dist.x += delta_dist.x;
                map.x += step.x;
                side = 0;
            } else {
                side_dist.y += delta_dist.y;
                map.y += step.y;
                side = 1;
            }

            pos = map.x + map.y * world.width;
        }

        double perp_wall_dist;
        // cálculo do comprimento do raio
        if (side == 0) {
            perp_wall_dist = fabs((map.x - ray_pos.x + (1 - step.x) / 2) / ray_dir.x);
        } else {
            perp_wall_dist = fabs((map.y - ray_pos.y + (1 - step.y) / 2) / ray_dir.y);
        }

        // cobrar RayHit com as informações para desenhar a coluna
        RayHit what;
        what.distance = perp_wall_dist;
        what.map = map;
        what.side = side;
        what.rayDir = ray_dir;

        // desenhe a coluna
        DrawColumn(what, world, pixel_canvas, column);
    }
}
