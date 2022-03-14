#include "chimera/core/Mesh.hpp"
#include <SDL2/SDL.h>

namespace Chimera {
void meshDebug(Mesh* m, bool _showAll) {

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Points: %03d Index: %03d", (int)m->point.size(), (int)m->iPoint.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normals: %03d Index: %03d", (int)m->normal.size(), (int)m->iNormal.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UVs:     %03d Index: %03d", (int)m->uv.size(), (int)m->iUv.size());

    if (_showAll == true) {
        for (uint32_t i = 0; i < m->point.size(); i++)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Point: %03d (%05.3f; %05.3f; %05.3f)", i, m->point[i].x, m->point[i].y, m->point[i].z);

        for (uint32_t i = 0; i < m->iPoint.size(); i += 3)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Point Index: %03d (%03d; %03d; %03d)", i, m->iPoint[i], m->iPoint[i + 1],
                         m->iPoint[i + 2]);

        for (uint32_t i = 0; i < m->normal.size(); i++)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal: %03d (%05.3f; %05.3f; %05.3f))", i, m->normal[i].x, m->normal[i].y,
                         m->normal[i].z);

        for (uint32_t i = 0; i < m->iNormal.size(); i += 3)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal Index: %03d (%03d; %03d; %03d)", i, m->iNormal[i], m->iNormal[i + 1],
                         m->iNormal[i + 2]);

        for (uint32_t i = 0; i < m->uv.size(); i++)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UV: %03d (%05.3f; %05.3f)", i, m->uv[i].x, m->uv[i].y);

        for (uint32_t i = 0; i < m->iUv.size(); i += 3)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UV Index: %03d (%03d; %03d; %03d)", i, m->iUv[i], m->iUv[i + 1], m->iUv[i + 2]);
    }
}
} // namespace Chimera