#pragma once
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/core/gl/Shader.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/space/AABB.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace ce {

    struct LinesValues {
        glm::vec3 point;
        glm::vec3 color;
    };

    class DrawLine {
      private:
        VertexArray* pVao{nullptr};
        VertexBuffer* pVbo{nullptr};
        std::shared_ptr<Shader> shader{nullptr};
        std::vector<LinesValues> points;

      public:
        DrawLine() noexcept = default;
        virtual ~DrawLine() noexcept { destroy(); };
        inline void add(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& color) noexcept {
            points.push_back({p0, color});
            points.push_back({p1, color});
        }
        void addAABB(const AABB& aabb, const glm::vec3& color) noexcept;
        void create(std::shared_ptr<Shader> shader, const uint32_t& sizeBuffer) noexcept;
        void destroy() noexcept;
        void render(MapUniform& uniformsQueue) noexcept;
        bool valid() noexcept { return pVao != nullptr; }
    };
} // namespace ce
