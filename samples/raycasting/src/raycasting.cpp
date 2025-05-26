#include "raycasting.hpp"
#include <cmath>

bool LoadWorld(const char filename[], World* world) {
    FILE* file;
    char string[1024];

    file = fopen(filename, "rb");
    if (!file) {
        return false;
    }

    // tamanho do mapa
    fgets(string, 1024, file);
    world->width = atoi(string);

    fgets(string, 1024, file);
    world->height = atoi(string);

    // alocação de mapa
    world->data = new uint8_t[world->width * world->height];

    // carregando mapa
    for (uint32_t h = 0; h < world->height; h++) {
        fgets(string, 1024, file);

        for (uint32_t w = 0; w < world->width; w++) {

            int indice = w + (h * world->width);

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

void DrawColumn(RayHit what, World world, ce::ICanva* frame, uint32_t column) {
    // tipo de bloco detectado
    uint8_t type = world.data[what.map.x + what.map.y * world.width];

    // selecione cor com base no tipo de bloco
    uint32_t corVal = 0;
    switch (type) {
        case 1:
            corVal = SDL_MapRGBA(SDL_GetPixelFormatDetails(frame->getPixelFormat()), NULL, 0, 255, 0, 0);
            break;
        case 2:
            corVal = SDL_MapRGBA(SDL_GetPixelFormatDetails(frame->getPixelFormat()), NULL, 155, 155, 155, 0);
            break;
        case 3:
            corVal = SDL_MapRGBA(SDL_GetPixelFormatDetails(frame->getPixelFormat()), NULL, 0, 0, 255, 0);
            break;
        case 4:
            corVal = SDL_MapRGBA(SDL_GetPixelFormatDetails(frame->getPixelFormat()), NULL, 255, 0, 0, 0);
            break;
    }

    // calcular a altura da coluna
    uint32_t colh = abs(int(frame->getHeight() / what.distance));
    uint32_t cropup = 0;
    uint32_t cropdown = 0;
    uint32_t index = 0;

    if (colh > frame->getHeight()) // se for maior que a tela, corte
    {
        index = column;
        cropup = (colh - frame->getHeight()) / 2;
        cropdown = cropup + 1;
    } else {
        index = column + ((frame->getHeight() - colh) / 2) * frame->getWidth();
        cropup = 0;
        cropdown = 0;
    }

    // desenhar coluna
    for (uint32_t c = cropup; c < (colh - cropdown); c++) {
        // desenhe o pixel da cor selecionada
        frame->getPixels()[index] = corVal;
        index += frame->getWidth();
    }
}

void RenderScene(State state, World world, ce::ICanva* frame) {

    for (uint32_t column = 0; column < frame->getWidth(); column++) // Para cada coluna
    {
        // calcular a posição e direção do feixe
        float cameraX = 2 * column / float(frame->getWidth()) - 1;
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
        while (world.data[map.x + map.y * world.width] == 0) // até nos encontrarmos com uma parede ...
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
        DrawColumn(what, world, frame, column);
    }
}
