#include "chimera/render/TransData.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

// void TransData::accept(NodeVisitor* v) { v->visit(this); }

glm::mat4 TransData::getModelMatrix(const glm::vec3& _position) { // ajuste matricial
    if (isStatic == false) {
        glm::mat4 matrixCoord = model;
        float* matrix = glm::value_ptr(matrixCoord);
        // pega posicao do objeto horigem de desenho (viewpoint fixo)
        // desloca desenha para o pbjeto horigem
        matrix[12] -= _position.x;
        matrix[13] -= _position.y;
        matrix[14] -= _position.z;
        return glm::make_mat4(matrix);
    }
    return model;
}

void TransData::setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation) {

    this->setPosition(_posicao);
    this->setRotation(_rotation);
}
} // namespace Chimera
