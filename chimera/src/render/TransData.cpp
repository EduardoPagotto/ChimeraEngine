#include "chimera/render/TransData.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

glm::mat4 TransData::getModelMatrix(const glm::vec3& _position) { // ajuste matricial
    glm::mat4 matrixCoord = model;
    float* matrix = glm::value_ptr(matrixCoord);
    // pega posicao do objeto horigem de desenho (viewpoint fixo)
    // desloca desenha para o pbjeto horigem
    matrix[12] -= _position.x;
    matrix[13] -= _position.y;
    matrix[14] -= _position.z;
    return glm::make_mat4(matrix);
}
} // namespace Chimera
