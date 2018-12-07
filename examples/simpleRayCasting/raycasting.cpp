#include <cmath>

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
    uint8_t type = world.data[what.mapX + what.mapY * world.width];

    // selecione cor com base no tipo de bloco
    uint8_t r, g, b;
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

        // FIXME: colocar em ordem o RGBA
        uint32_t valR = r << 16; //(0x00FF0000 & r);
        uint32_t valG = g << 8;  //(0x0000FF00 & g);
        uint32_t valB = b;       //(0x000000FF & b);

        frame.data[index] = (valR | valG | valB);
        // frame.data[index].r = r;
        // frame.data[index].g = g;
        // frame.data[index].b = b;

        index += frame.width;
    }
}

void RenderScene(State state, World world, Frame frame) {
    uint32_t i = 0;

    int tamanho_frame = (frame.width * frame.height);
    for (i = 0; i < tamanho_frame; i++) {

        // // FIXME: colocar em ordem o RGBA
        uint32_t valZero = 0;
        frame.data[i] = 0;
        // frame.data[i].r = 0;
        // frame.data[i].g = 0;
        // frame.data[i].b = 0;
    }

    for (uint32_t column = 0; column < frame.width; column++) // Para cada coluna
    {
        // calcular a posição e direção do feixe
        double cameraX = 2 * column / double(frame.width) - 1;
        double rayPosX = state.posx;
        double rayPosY = state.posy;
        double rayDirX = state.dirx + state.camx * cameraX;
        double rayDirY = state.diry + state.camy * cameraX;

        // o bloco atual onde estamos
        int mapX = int(rayPosX);
        int mapY = int(rayPosY);

        // comprimento do feixe da posição atual para o próximo bloco
        double sideDistX;
        double sideDistY;

        // comprimento do raio de um bloco para outro
        double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
        double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
        double perpWallDist;

        // direção para onde ir (+1 ou -1), tanto para X quanto para Y
        int stepX;
        int stepY;
        int side; // face do cubo encontrado (face Norte-Sul ou face Oeste-Leste)

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (rayPosX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
        }

        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (rayPosY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
        }

        // vamos lançar o raio
        while ((world.data[mapX + mapY * world.width] == 0)) // até nos encontrarmos com uma parede ...
        {
            // vamos para o próximo bloco no mapa
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
        }

        // cálculo do comprimento do raio
        if (side == 0) {
            perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
        } else {
            perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);
        }

        // cobrar RayHit com as informações para desenhar a coluna
        RayHit what;
        what.distance = perpWallDist;
        what.mapX = mapX;
        what.mapY = mapY;
        what.side = side;
        what.rayDirX = rayDirX;
        what.rayDirY = rayDirY;

        // desenhe a coluna
        DrawColumn(what, world, state, frame, column);
    }
}

// void DrawScene(Display& display, Frame frame) { display.update(frame.data); }