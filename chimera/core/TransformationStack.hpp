#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace ce {

class TransformationStack {
  public:
    TransformationStack() {
        transformationStack.push_back(glm::mat4(1.0f));
        transCache = &transformationStack.back();
    }

    inline void push(const glm::mat4& matrix) {
        transformationStack.push_back(transformationStack.back() * matrix);
        transCache = &transformationStack.back();
    }

    inline void pushOver(const glm::mat4& matrix) {
        transformationStack.push_back(matrix);
        transCache = &transformationStack.back();
    }

    inline const glm::vec3 multiplVec3(const glm::vec3& point) const { return glm::vec3((*transCache) * glm::vec4(point, 1.0f)); }

    inline void pop() {
        if (transformationStack.size() > 1)
            transformationStack.pop_back();

        transCache = &transformationStack.back();
        // TODO: log here!!!
    }

    inline glm::mat4 const get() const { return *transCache; }

  private:
    std::vector<glm::mat4> transformationStack;
    const glm::mat4* transCache;
};
} // namespace ce
