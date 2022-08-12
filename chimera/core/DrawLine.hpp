#pragma once
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
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
    void add(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& color) {
        points.push_back({p0, color});
        points.push_back({p1, color});
    }
    void init();
    void render(const glm::mat4& projection, const glm::mat4& view);

    void addAABB(const AABB& aabb, const glm::vec3& color);

  private:
    Shader shader;
    VertexArray vao;
    VertexBuffer* vbo;
    std::vector<LinesValues> points;
};
} // namespace Chimera