#ifndef __CHIMERA_RENDERABLE_2D__HPP
#define __CHIMERA_RENDERABLE_2D__HPP

#include "chimera/core/Shader.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include <deque>

namespace Chimera {

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

    inline glm::vec3 multiplVec3(const glm::vec3& point) const { return glm::vec3((*transCache) * glm::vec4(point, 1.0f)); }

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

class IRenderable2D {
  public:
    virtual void submit(class IRenderer2D* renderer) = 0;
    virtual glm::vec3 getPosition() const = 0;
    virtual glm::vec2 getSize() const = 0;
    virtual glm::vec4 getColor() const = 0;
};

class IRenderer2D {
  public:
    virtual void begin() = 0;
    virtual void submit(IRenderable2D* renderable) = 0;
    virtual void end() = 0;
    virtual void flush() = 0;
    virtual TransformationStack& getStack() = 0;
};

class Renderable2D : public IRenderable2D {
  public:
    Renderable2D(glm::vec3 pos, glm::vec2 size, glm::vec4 color);
    virtual ~Renderable2D();
    virtual void submit(class IRenderer2D* renderer) override { renderer->submit(this); }
    inline glm::vec3 getPosition() const override { return pos; }
    inline glm::vec2 getSize() const override { return size; }
    inline glm::vec4 getColor() const override { return color; }

  private:
    glm::vec3 pos;
    glm::vec2 size;
    glm::vec4 color;
};
} // namespace Chimera
#endif