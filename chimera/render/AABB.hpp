#include <glm/glm.hpp>

namespace chimera {

class AABB {
  public:
    AABB();
    ~AABB();
    void set(const glm::vec3& _min, const glm::vec3& _max);
    bool pointInside(const glm::vec3& _point);
    // bool Visible(CFrustum &Frustum);
    // float Distance(CFrustum &Frustum);
    void render();

  private:
    glm::vec3 vertices[8];
};
} // namespace chimera