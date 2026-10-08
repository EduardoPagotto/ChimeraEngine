#pragma once
#include "AABB.hpp"
#include <string>
#include <vector>

namespace ce {

    struct VertexData {
        glm::vec3 point;  // 3 * 4 = 12 ( 0 - 11)
        glm::vec3 normal; // 3 * 4 = 12 (12 - 23)
        glm::vec2 uv;     // 2 * 4 = 08 (24 - 31)
    };

    struct Vertex3D {
        glm::vec3 point{0.0};
        glm::vec3 normal{0.0};
        glm::vec2 uv{0.0};
    };

    struct SubMesh3D {
        AABB aabb;
        std::vector<uint32_t> indices;
        size_t mat_indice;
    };

    struct Mesh3D {
        std::string name;
        std::vector<Vertex3D> vertices;
        std::vector<SubMesh3D> subs; // Each part represents a glTF primitive
    };

    // TODO: subistituir no futuro
    // // Auxiliar triângulo na CPU para ler indices
    // glm::ivec3 getTriangle(size_t faceIndex, uint32_t* indices) {
    //     const uint32_t a = faceIndex * 3;
    //     return {indices[a], indices[a + 1], indices[a + 2]};
    // }

    // // Auxiliar triângulo na CPU para gravar indices
    // void setTriangle(glm::ivec3 face, size_t faceIndex, uint32_t* indices) {
    //     const uint32_t a = faceIndex * 3;
    //     indices[a] = face.x;
    //     indices[a + 1] = face.y;
    //     indices[a + 2] = face.z;
    // }

} // namespace ce
