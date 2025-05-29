#pragma once
#include "Frustum.hpp"
#include <vector>

namespace ce {

    enum class AabbBondery { BSW = 0, BSE = 1, TSW = 2, TSE = 3, BNW = 4, BNE = 5, TNW = 6, TNE = 7 };

    class AABB { // ref: http://www.3dcpptutorials.sk/index.php?id=59
      protected:
        std::vector<glm::vec3> vertex;

      private:
        glm::vec3 position{0.0f};
        glm::vec3 size{0.0f};

      public:
        AABB() noexcept = default;

        AABB(const AABB& _cpy) noexcept = default;

        AABB(const glm::vec3& min, const glm::vec3& max) noexcept { this->setBoundary(min, max); }

        virtual ~AABB() = default;

        inline const bool visible(const Frustum& _frustum) const noexcept { return _frustum.AABBVisible(vertex); }

        inline const glm::vec3 getPosition() const { return position; }

        inline const glm::vec3 getSize() const { return size; }

        inline void setPosition(const glm::vec3& pos, const glm::vec3& size) noexcept {
            const glm::vec3 halfV = (size / 2.0f);
            setBoundary((pos - halfV), (pos + halfV)); // min and max
        }

        inline void setBoundary(const glm::vec3& min, const glm::vec3& max) noexcept {

            if (!vertex.empty())
                vertex.clear();

            vertex.push_back(glm::vec3(min.x, min.y, min.z)); // AabbBondery::BSW 0 Minimal point (front)
            vertex.push_back(glm::vec3(max.x, min.y, min.z)); // AabbBondery::BSE 1
            vertex.push_back(glm::vec3(min.x, max.y, min.z)); // AabbBondery::TSW 2
            vertex.push_back(glm::vec3(max.x, max.y, min.z)); // AabbBondery::TSE 3
            vertex.push_back(glm::vec3(min.x, min.y, max.z)); // AabbBondery::BNW 4
            vertex.push_back(glm::vec3(max.x, min.y, max.z)); // AabbBondery::BNE 5
            vertex.push_back(glm::vec3(min.x, max.y, max.z)); // AabbBondery::TNW 6
            vertex.push_back(glm::vec3(max.x, max.y, max.z)); // AabbBondery::TNE 7 Maximal point (back)

            // TODO: Era half size ??
            this->size = getSizeMinMax(min, max);
            this->position = min + (this->size / 2.0f);
        }

        inline const bool intersects(const AABB& val) const noexcept {
            return (val.vertex[0].x > vertex[7].x || val.vertex[7].x < vertex[0].x || val.vertex[0].y < vertex[7].y ||
                    val.vertex[7].y < vertex[0].y || val.vertex[0].z < vertex[7].z || val.vertex[7].z < vertex[0].z);
        }

        inline const bool contains(const glm::vec3& val) const noexcept {
            return !((val.x < vertex[0].x) || (val.y < vertex[0].y) || (val.z < vertex[0].z) || (val.x > vertex[7].x) ||
                     (val.y > vertex[7].y) || (val.z > vertex[7].z));
        }

        inline const AABB transformation(const glm::mat4& transformation) const noexcept {
            // TODO: TESTAR!! ref: https://www.gamedev.net/forums/topic/673361-axis-aligned-boxes-and-rotations/
            glm::vec3 val, min, max;

            val = glm::vec3(transformation * glm::vec4(vertex[0], 1.0f));
            min = val;
            max = val;

            for (const glm::vec3& point : vertex) {
                val = glm::vec3(transformation * glm::vec4(point, 1.0f));
                min = glm::min(min, val);
                max = glm::max(max, val);
            }

            return AABB(min, max);
        }

        const std::vector<glm::vec3>& getAllVertex() const { return vertex; }

        // FIXME: dot normal posicao para distancia (calcula do AABB em relacao ao frustum)
        // inline const float distance(const Frustum& _frustum) const noexcept { return _frustum.AABBDistance(vertex); }
    };
} // namespace ce
