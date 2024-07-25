#pragma once
#include "chimera/core/ServiceLocator.hpp"
#include <array>
#include <glm/glm.hpp>

namespace Chimera {

struct ViewProjectionMatrixs {
    glm::mat4 view, viewProjection, viewProjectionInverse;
    ViewProjectionMatrixs() = default;
    void update(const glm::mat4& _view, const glm::mat4& _projection) {
        view = _view;
        viewProjection = _projection * _view;
        viewProjectionInverse = glm::inverse(_view) * glm::inverse(_projection);
    }
};

class IViewProjection : public IService {
  public:
    virtual ~IViewProjection() = default;
    virtual const float getNoze() const = 0;
    virtual void setNoze(const float& noze) = 0;
    virtual void setIndex(const uint8_t s) = 0;
    virtual const uint8_t getSize() const = 0;
    virtual ViewProjectionMatrixs& getSel() = 0;
    virtual ViewProjectionMatrixs& getLeft() = 0;  // 0
    virtual ViewProjectionMatrixs& getRight() = 0; // 1
};

class ViewProjection : public ServiceBase<IViewProjection> {
  public:
    explicit ViewProjection() = default;
    explicit ViewProjection(const float& noze) { this->setNoze(noze); }
    ViewProjection(const ViewProjection& o) = delete;
    ViewProjection& operator=(const ViewProjection& o) = delete;
    virtual ~ViewProjection() = default;
    virtual const float getNoze() const { return noze; }
    virtual void setNoze(const float& noze) {
        this->noze = noze;
        size = (noze == 0.0f) ? 1 : 2;
    }
    virtual void setIndex(const uint8_t s) { indice = (s >= 0 && s < 2) ? s : 0; }
    virtual const uint8_t getSize() const { return size; }
    virtual ViewProjectionMatrixs& getSel() { return vpm[indice]; }
    virtual ViewProjectionMatrixs& getLeft() { return vpm[0]; }  // 0
    virtual ViewProjectionMatrixs& getRight() { return vpm[1]; } // 1

  private:
    float noze{0.0f};
    uint8_t indice{0};
    uint8_t size{1};
    std::array<ViewProjectionMatrixs, 2> vpm;
};

struct ViewProjectionComponent {
    ViewProjectionComponent() = default;
    IViewProjection* vp = nullptr;
};

} // namespace Chimera