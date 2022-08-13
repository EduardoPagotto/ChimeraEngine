#pragma once
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
#include "chimera/core/visible/Shader.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

struct LinesValues {
    glm::vec3 point;
    glm::vec3 color;
};

class DrawLine {
  public:
    DrawLine();
    ~DrawLine();
    inline void add(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& color) {
        points.push_back({p0, color});
        points.push_back({p1, color});
    }
    void addAABB(const AABB& aabb, const glm::vec3& color);
    void create(const uint32_t& sizeBuffer);
    void destroy();
    void render(RenderCommand& command, MapUniform& uniformsQueue);

  private:
    Shader shader;
    VertexArray* pVao;
    VertexBuffer* pVbo;
    std::vector<LinesValues> points;
};
} // namespace Chimera