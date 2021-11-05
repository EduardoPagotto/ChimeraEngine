#include "chimera/render/Transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Chimera {

glm::mat4 Transform::getModelMatrix(const glm::vec3& _position) { // ajuste matricial
    glm::mat4 matrixCoord = transform;
    float* matrix = glm::value_ptr(matrixCoord);
    // pega posicao do objeto horigem de desenho (viewpoint fixo)
    // desloca desenha para o pbjeto horigem
    matrix[12] -= _position.x;
    matrix[13] -= _position.y;
    matrix[14] -= _position.z;
    return glm::make_mat4(matrix);
}

// void Transform::setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation) {
//     glm::quat myQuat(_rotation);            // trocar (pitch, yaw, roll) por (yaw, pitch, roll) ?????
//     glm::mat4 matRot = glm::toMat4(myQuat); // matriz rotacao
//     glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), _posicao); // matriz translacao
//     transform = matRot * matTrans; // primeiro translada depois rotaciona, ordem é importante!!!
// }

} // namespace Chimera
