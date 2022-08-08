#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

struct ViewProjectionMatrixs {
    std::string name;
    glm::mat4 view, viewProjection, viewProjectionInverse;
    ViewProjectionMatrixs(const std::string& name) : name(name) {}
    void update(const glm::mat4& _view, const glm::mat4& _projection) {
        view = _view;
        viewProjection = _projection * _view;
        viewProjectionInverse = glm::inverse(_view) * glm::inverse(_projection);
    }
};

class ViewProjection {
  public:
    ViewProjection() : index(0), noseDist(0.4f) {}
    void setIndex(const uint8_t& index) { this->index = index; }
    void setDist(const float& dist) { this->noseDist = dist; }
    const uint8_t getIndex() const { return index; }
    const glm::mat4& getView() const { return head[index].view; }
    const glm::mat4& getViewProjection() const { return head[index].viewProjection; }
    const glm::mat4& getViewProjectionInverse() const { return head[index].viewProjectionInverse; }
    const float& getNoseDist() const { return noseDist; }
    void update(const glm::mat4& view, const glm::mat4& projection) { head[index].update(view, projection); }
    void add(const std::string& name) { head.push_back(ViewProjectionMatrixs(name)); }
    const uint32_t size() const { return head.size(); }
    std::vector<ViewProjectionMatrixs>& getHead() { return head; }

  private:
    uint8_t index;
    float noseDist;
    std::vector<ViewProjectionMatrixs> head;
};
} // namespace Chimera