#include "chimera/render/VertexData.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

std::tuple<glm::vec3, glm::vec3, glm::vec3> vertexBoundaries(std::vector<VertexData>& vArray) {
    glm::vec3 min, max, size;
    if (vArray.size() > 0) {
        min = vArray[0].point;
        max = vArray[0].point;
    }

    for (const VertexData& v : vArray) {
        min = glm::min(min, v.point);
        max = glm::max(max, v.point);
    }

    return {min, max, getSizeMinMax(min, max)};
}

std::tuple<glm::vec3, glm::vec3, glm::vec3> vertexIndexedBoundaries(std::vector<VertexData>& vArray, TrisIndex& tris) {
    glm::vec3 min, max;
    if (tris.size() > 0) {
        max = min = vArray[tris[0].x].point;
        max = min = vArray[tris[0].y].point;
        max = min = vArray[tris[0].z].point;
    }

    for (const glm::uvec3& i : tris) {
        min = glm::min(min, vArray[i.x].point);
        min = glm::min(min, vArray[i.y].point);
        min = glm::min(min, vArray[i.z].point);

        max = glm::max(max, vArray[i.x].point);
        max = glm::max(max, vArray[i.y].point);
        max = glm::max(max, vArray[i.z].point);
    }

    return {min, max, getSizeMinMax(min, max)};
}
} // namespace Chimera