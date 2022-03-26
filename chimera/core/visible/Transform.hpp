#pragma once

#include "ITrans.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Chimera {
class Transform : public ITrans {
  public:
    Transform() = default;
    Transform(const glm::mat4& transform) : transform(transform) {}
    virtual ~Transform() = default;
    virtual const glm::vec3 getPosition() const override { return glm::vec3(this->transform[3]); }
    virtual const glm::mat4 getMatrix() const override { return this->transform; }
    virtual const glm::mat4 translateSrc(const glm::vec3& pos) const override {
        glm::mat4 matrixCoord = transform;
        float* matrix = glm::value_ptr(matrixCoord);
        // pega posicao do objeto horigem de desenho (viewpoint fixo),desloca desenha para o pbjeto horigem
        matrix[12] -= pos.x;
        matrix[13] -= pos.y;
        matrix[14] -= pos.z;
        return glm::make_mat4(matrix);
    }
    // void setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation) {
    //     glm::quat myQuat(_rotation);                                    // trocar (pitch, yaw, roll) por (yaw, pitch, roll) ?????
    //     glm::mat4 matRot = glm::toMat4(myQuat);                         // matriz rotacao
    //     glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), _posicao); // matriz translacao
    //     transform = matRot * matTrans;                                  // primeiro translada depois rotaciona, ordem é importante!!!
    // }
    virtual void setPosition(const glm::vec3& pos) override { this->transform = glm::translate(this->transform, pos); }
    virtual void setRotation(const glm::vec3& rot) override { transform = glm::eulerAngleYXZ(rot.y, rot.x, rot.z); }
    virtual void setMatrix(const glm::mat4& transform) override { this->transform = transform; }

  private:
    glm::mat4 transform = glm::mat4(1.0f);
};
} // namespace Chimera
