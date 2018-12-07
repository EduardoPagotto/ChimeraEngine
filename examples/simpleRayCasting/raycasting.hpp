#include <stdint.h>
#include <string>

// ref: http://www.gianlucaghettini.net/motore-3d-in-c-raycasting/

// Field Of View
#define FOV 0.66

// velocita' di movimento e rotazione
#define MOVSPEED 0.1
#define ROTSPEED 0.05

// framebuffer
struct Frame {
    uint32_t width;
    uint32_t height;
    uint32_t* data; // TrueColorPixel* data;
};

// mappa
struct World {
    uint32_t width;
    uint32_t height;
    uint8_t* data;
};

// stato della telecamera (posizione, direzione, piano di proiezione)
struct State {
    double posx;
    double posy;
    double dirx;
    double diry;
    double camx;
    double camy;
};

// informazioni sul raggio
struct RayHit {
    double distance;
    int mapX;
    int mapY;
    double rayDirX;
    double rayDirY;
    int side;
};

bool LoadWorld(const char filename[], World* world);
void DrawColumn(RayHit what, World world, State state, Frame frame, uint32_t column);
void RenderScene(State state, World world, Frame frame);
// void DrawScene(Display& display, Frame frame)