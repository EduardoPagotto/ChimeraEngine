#pragma once
#include <stdint.h>
#include <string>

#include <glm/glm.hpp>
// ref: http://www.gianlucaghettini.net/motore-3d-in-c-raycasting/

// Field Of View
#define FOV 0.66

// velocidade de movimento e rotação
#define MOVSPEED 0.1
#define ROTSPEED 0.05

// framebuffer
struct Frame {
    uint32_t width;
    uint32_t height;
    uint32_t* data; // TrueColorPixel* data;
};

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
void DrawColumn(RayHit what, World world, State state, Frame frame, uint32_t column);
void RenderScene(State state, World world, Frame frame);
