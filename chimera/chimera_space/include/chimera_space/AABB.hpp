#pragma once
#include "Frustum.hpp"
#include <array>

namespace ce {

    enum class AabbBondery { BSW = 0, BSE = 1, TSW = 2, TSE = 3, BNW = 4, BNE = 5, TNW = 6, TNE = 7 };

    enum class AabbLimits {
        LX_LY_LZ = 0,
        HX_LY_LZ = 1,
        LX_HY_LZ = 2,
        HX_HY_LZ = 3,
        LX_LY_HZ = 4,
        HX_LY_HZ = 5,
        LX_HY_HZ = 6,
        HX_HY_HZ = 7
    };

    class AABB { // ref: http://www.3dcpptutorials.sk/index.php?id=59
      protected:
        std::array<glm::vec3, 8> vertex;

      private:
        glm::vec3 position{0.0F};
        glm::vec3 size{0.0F};

      public:
        explicit AABB() noexcept = default;
        explicit AABB(const AABB& _cpy) noexcept = default;
        explicit AABB(const glm::vec3& min, const glm::vec3& max) noexcept { this->setBoundary(min, max); }

        virtual ~AABB() = default;

        bool visible(const Frustum& _frustum) const noexcept { return _frustum.aabbVisible(vertex); }

        glm::vec3 getPosition() const { return position; }

        glm::vec3 getSize() const { return size; }

        void setPosition(const glm::vec3& pos, const glm::vec3& size) noexcept {
            const glm::vec3 halfV = (size / 2.0F);
            setBoundary((pos - halfV), (pos + halfV)); // min and max
        }

        void setBoundary(const glm::vec3& min, const glm::vec3& max) noexcept {

            vertex[0] = min; // glm::vec3(min.x, min.y, min.z); // AabbBondery::BSW 0 Minimal point (front)
            vertex[1] = glm::vec3(max.x, min.y, min.z); // AabbBondery::BSE 1
            vertex[2] = glm::vec3(min.x, max.y, min.z); // AabbBondery::TSW 2
            vertex[3] = glm::vec3(max.x, max.y, min.z); // AabbBondery::TSE 3
            vertex[4] = glm::vec3(min.x, min.y, max.z); // AabbBondery::BNW 4
            vertex[5] = glm::vec3(max.x, min.y, max.z); // AabbBondery::BNE 5
            vertex[6] = glm::vec3(min.x, max.y, max.z); // AabbBondery::TNW 6
            vertex[7] = max; // glm::vec3(max.x, max.y, max.z); // AabbBondery::TNE 7 Maximal point (back)

            this->size = getSizeMinMax(min, max);
            this->position = min + (this->size / 2.0F);
        }

        bool intersects(const AABB& val) const noexcept {
            return (val.vertex[0].x > vertex[7].x || val.vertex[7].x < vertex[0].x || val.vertex[0].y < vertex[7].y ||
                    val.vertex[7].y < vertex[0].y || val.vertex[0].z < vertex[7].z || val.vertex[7].z < vertex[0].z);
        }

        bool contains(const glm::vec3& val) const noexcept {
            // TODO: TESTAR!!
            return (val.x >= vertex[0].x) && (val.y >= vertex[0].y) && (val.z >= vertex[0].z) &&
                   (val.x <= vertex[7].x) && (val.y <= vertex[7].y) && (val.z <= vertex[7].z); // NOLINT
        }

        AABB transformation(const glm::mat4& transformation) const noexcept {
            // TODO: TESTAR!! ref: https://www.gamedev.net/forums/topic/673361-axis-aligned-boxes-and-rotations/
            glm::vec3 val{glm::vec3(transformation * glm::vec4(vertex[0], 1.0F))};
            glm::vec3 min{val};
            glm::vec3 max{val};

            for (const glm::vec3& point : vertex) {
                val = glm::vec3(transformation * glm::vec4(point, 1.0F));
                min = glm::min(min, val);
                max = glm::max(max, val);
            }

            return AABB(min, max);
        }

        const std::array<glm::vec3, 8>& getAllVertex() const { return vertex; }

        // FIXME: dot normal posicao para distancia (calcula do AABB em relacao ao frustum)
        // float distance(const Frustum& _frustum) const noexcept { return _frustum.AABBDistance(vertex); }
    };
} // namespace ce
