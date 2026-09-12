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

void DrawColumn(RayHit what, World world, ce::CanvaFB* canva, uint32_t column) {
    // tipo de bloco detectado

    auto pos = what.map.x + what.map.y * world.width;
    if (pos > world.data.size()) {
        throw std::runtime_error(std::format("Poiscao do mapa invalida: {}", pos));
    }

    uint8_t type = world.data[pos];

    const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(canva->getPixelFormat());
    // const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_BGRA32);

    // selecione cor com base no tipo de bloco
    uint32_t corVal = 0xffffff;

    switch (type) {
        case 1:
            corVal = SDL_MapRGBA(details, NULL, 0, 255, 0, 0);
            break;
        case 2:
            corVal = SDL_MapRGBA(details, NULL, 155, 155, 155, 0);
            break;
        case 3:
            corVal = SDL_MapRGBA(details, NULL, 0, 0, 255, 0);
            break;
        case 4:
            corVal = SDL_MapRGBA(details, NULL, 255, 0, 0, 0);
            break;
        default:
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Pixel incompativel");
            break;
    }

    // calcular a altura da coluna
    uint32_t colh = abs(int(canva->getHeight() / what.distance));
    uint32_t cropup = 0;
    uint32_t cropdown = 0;
    uint32_t index = 0;

    if (colh > canva->getHeight()) // se for maior que a tela, corte
    {
        index = column;
        cropup = (colh - canva->getHeight()) / 2;
        cropdown = cropup + 1;
    } else {
        index = column + (((canva->getHeight() - colh) / 2) * canva->getWidth());
        cropup = 0;
        cropdown = 0;
    }

    // desenhar coluna
    for (uint32_t c = cropup; c < (colh - cropdown); c++) {
        // desenhe o pixel da cor selecionada
        canva->getPixels()[index] = corVal | 0xfffff; // corVal ; // 0xffffff; //
        index += canva->getWidth();
    }
}

void RenderScene(State state, World world, ce::CanvaFB* canva) {

    for (uint32_t column = 0; column < canva->getWidth(); column++) // Para cada coluna
    {
        // calcular a posição e direção do feixe
        float cameraX = 2 * column / float(canva->getWidth()) - 1;
        glm::vec2 rayPos = state.pos;
        glm::vec2 rayDir = state.dir + state.cam * cameraX;

        // o bloco atual onde estamos
        glm::ivec2 map = rayPos;

        // comprimento do feixe da posição atual para o próximo bloco
        glm::vec2 sideDist;

        // comprimento do raio de um bloco para outro
        glm::vec2 deltaDist(sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x)),
                            sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y)));

        // direção para onde ir (+1 ou -1), tanto para X quanto para Y
        glm::ivec2 step(0, 0);
        if (rayDir.x < 0) {
            step.x = -1;
            sideDist.x = (rayPos.x - map.x) * deltaDist.x;
        } else {
            step.x = 1;
            sideDist.x = (map.x + 1.0 - rayPos.x) * deltaDist.x;
        }

        if (rayDir.y < 0) {
            step.y = -1;
            sideDist.y = (rayPos.y - map.y) * deltaDist.y;
        } else {
            step.y = 1;
            sideDist.y = (map.y + 1.0 - rayPos.y) * deltaDist.y;
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
            if (sideDist.x < sideDist.y) {
                sideDist.x += deltaDist.x;
                map.x += step.x;
                side = 0;
            } else {
                sideDist.y += deltaDist.y;
                map.y += step.y;
                side = 1;
            }

            pos = map.x + map.y * world.width;
        }

        double perpWallDist;
        // cálculo do comprimento do raio
        if (side == 0) {
            perpWallDist = fabs((map.x - rayPos.x + (1 - step.x) / 2) / rayDir.x);
        } else {
            perpWallDist = fabs((map.y - rayPos.y + (1 - step.y) / 2) / rayDir.y);
        }

        // cobrar RayHit com as informações para desenhar a coluna
        RayHit what;
        what.distance = perpWallDist;
        what.map = map;
        what.side = side;
        what.rayDir = rayDir;

        // desenhe a coluna
        DrawColumn(what, world, canva, column);
    }
}
