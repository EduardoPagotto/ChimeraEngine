#include "chimera/render/3d/PolygonIndex.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

PolygonIndex::PolygonIndex() : ibo(nullptr) {}

PolygonIndex::~PolygonIndex() {

    polygons.clear();
    if (ibo) {
        delete ibo;
        ibo = nullptr;
    }
}

void PolygonIndex::addTrisEntire(uint32_t* indexData, const uint32_t& indexSize) {
    for (uint32_t i = 0; i < indexSize; i += 3)
        addTris(indexData[i], indexData[i + 1], indexData[i + 2]);
}

void PolygonIndex::addTris(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc) {
    polygons.push_back(_pa);
    polygons.push_back(_pb);
    polygons.push_back(_pc);
}

void PolygonIndex::initializeBuffer(VertexData* vertexData, const uint32_t& vertexSize) {
    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(vertexData, vertexSize, &polygons[0], polygons.size(), min, max, size);
    aabb.setBoundary(min, max);

    ibo = new IndexBuffer(&polygons[0], polygons.size());
}

void PolygonIndex::debugDados() const {
    glm::vec3 size = aabb.getSize();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "IBO: %d Faces: %d AABB[ %.2f, %.2f, %.2f]", this->ibo->getBufferID(), this->getSize() / 3,
                 size.x, size.y, size.z);

    aabb.render();
}

} // namespace Chimera