#include <cmath>

#include <SDL2/SDL.h>

#include "raycasting.hpp"

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

void DrawColumn(RayHit what, World world, State state, Frame frame, uint32_t column) {
    // tipo de bloco detectado
    uint8_t type = world.data[what.map.x + what.map.y * world.width];

    // selecione cor com base no tipo de bloco
    uint8_t r, g, b, a;
    a = 0;
    switch (type) {
        case 1: {
            r = 0;
            g = 255;
            b = 0;
            break;
        }
        case 2: {
            r = 155;
            g = 155;
            b = 155;
            break;
        }
        case 3: {
            r = 0;
            g = 0;
            b = 255;
            break;
        }
        case 4: {
            r = 255;
            g = 0;
            b = 0;
            break;
        }
    }

    // calcular a altura da coluna

    uint32_t colh = abs(int(frame.height / what.distance));

    uint32_t cropup = 0;
    uint32_t cropdown = 0;
    uint32_t index = 0;

    if (colh > frame.height) // se for maior que a tela, corte
    {
        index = column;
        cropup = (colh - frame.height) / 2;
        cropdown = cropup + 1;
    } else {
        index = column + ((frame.height - colh) / 2) * frame.width;
        cropup = 0;
        cropdown = 0;
    }

    // desenhar coluna
    for (uint32_t c = cropup; c < (colh - cropdown); c++) {
        // desenhe o pixel da cor selecionada

        uint32_t valor_final = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), r, g, b, a);
        frame.data[index] = valor_final;

        index += frame.width;
    }
}

void RenderScene(State state, World world, Frame frame) {

    for (uint32_t column = 0; column < frame.width; column++) // Para cada coluna
    {
        // calcular a posição e direção do feixe
        double cameraX = 2 * column / double(frame.width) - 1;
        glm::vec2 rayPos = state.pos;
        glm::vec2 rayDir(state.dir.x + state.cam.x * cameraX, state.dir.y + state.cam.y * cameraX);

        // o bloco atual onde estamos
        // int mapX = int(rayPos.x);
        // int mapY = int(rayPos.y);
        glm::ivec2 map(int(rayPos.x), int(rayPos.y));

        // comprimento do feixe da posição atual para o próximo bloco
        double sideDistX;
        double sideDistY;

        // comprimento do raio de um bloco para outro
        double deltaDistX = sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x));
        double deltaDistY = sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y));
        double perpWallDist;

        // direção para onde ir (+1 ou -1), tanto para X quanto para Y
        int stepX;
        int stepY;
        int side; // face do cubo encontrado (face Norte-Sul ou face Oeste-Leste)

        if (rayDir.x < 0) {
            stepX = -1;
            sideDistX = (rayPos.x - map.x) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (map.x + 1.0 - rayPos.x) * deltaDistX;
        }

        if (rayDir.y < 0) {
            stepY = -1;
            sideDistY = (rayPos.y - map.y) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (map.y + 1.0 - rayPos.y) * deltaDistY;
        }

        // vamos lançar o raio
        while ((world.data[map.x + map.y * world.width] == 0)) // até nos encontrarmos com uma parede ...
        {
            // vamos para o próximo bloco no mapa
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                map.x += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                map.y += stepY;
                side = 1;
            }
        }

        // cálculo do comprimento do raio
        if (side == 0) {
            perpWallDist = fabs((map.x - rayPos.x + (1 - stepX) / 2) / rayDir.x);
        } else {
            perpWallDist = fabs((map.y - rayPos.y + (1 - stepY) / 2) / rayDir.y);
        }

        // cobrar RayHit com as informações para desenhar a coluna
        RayHit what;
        what.distance = perpWallDist;
        what.map.x = map.x;
        what.map.y = map.y;
        what.side = side;
        what.rayDir = rayDir;

        // desenhe a coluna
        DrawColumn(what, world, state, frame, column);
    }
}

// void DrawScene(Display& display, Frame frame) { display.update(frame.data); }