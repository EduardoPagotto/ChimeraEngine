#pragma once
#include "chimera_core/gl/Shader.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"
#include "chimera_space/AABB.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace ce {

    struct LinesValues {
        glm::vec3 point;
        glm::vec3 color;
    };

    class DrawLine {
      public:
        DrawLine() noexcept = default;
        virtual ~DrawLine() noexcept { destroy(); };
        inline void add(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& color) noexcept {
            points_.push_back({p0, color});
            points_.push_back({p1, color});
        }
        void add_aabb(const AABB& aabb, const glm::vec3& color) noexcept;
        void create(std::shared_ptr<Shader> shader, const uint32_t& size_buffer) noexcept;
        void destroy() noexcept;
        void render(MapUniform& uniforms_queue) noexcept;
        bool valid() noexcept { return vao_ != nullptr; }

      private:
        std::shared_ptr<VertexArray> vao_;
        std::shared_ptr<VertexBuffer> vbo_;
        std::shared_ptr<Shader> shader_;
        std::vector<LinesValues> points_;
    };
} // namespace ce
