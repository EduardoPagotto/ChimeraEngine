#pragma once
#include "chimera/core/device/ICanva.hpp"
#include <glm/glm.hpp>
#include <stdint.h>
#include <string>
// ref: http://www.gianlucaghettini.net/motore-3d-in-c-raycasting/
// Field Of View
#define FOV 0.66
// mapa
struct World {
    uint32_t width;
    uint32_t height;
    uint8_t* data;
};
// estado da câmera (posição, direção, plano de projeção)
struct State {
    glm::vec2 pos;
    glm::vec2 dir;
    glm::vec2 cam;
};
// informação de raio
struct RayHit {
    double distance;
    glm::ivec2 map;
    glm::vec2 rayDir;
    int side;
};

bool LoadWorld(const char filename[], World* world);
void DrawColumn(RayHit what, World world, Chimera::Canvas* frame, uint32_t column);
void RenderScene(State state, World world, Chimera::Canvas* frame);
