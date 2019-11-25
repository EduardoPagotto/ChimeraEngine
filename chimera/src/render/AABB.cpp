#include "chimera/render/AABB.hpp"
#include "chimera/OpenGLDefs.hpp"
#include <glm/gtc/type_ptr.hpp>

// ref: http://www.3dcpptutorials.sk/index.php?id=59

namespace Chimera {

AABB::AABB() {}

AABB::~AABB() {}

void AABB::set(const glm::vec3& _min, const glm::vec3& _max) {
    vertices[0] = glm::vec3(_min.x, _min.y, _min.z);
    vertices[1] = glm::vec3(_max.x, _min.y, _min.z);
    vertices[2] = glm::vec3(_min.x, _max.y, _min.z);
    vertices[3] = glm::vec3(_max.x, _max.y, _min.z);
    vertices[4] = glm::vec3(_min.x, _min.y, _max.z);
    vertices[5] = glm::vec3(_max.x, _min.y, _max.z);
    vertices[6] = glm::vec3(_min.x, _max.y, _max.z);
    vertices[7] = glm::vec3(_max.x, _max.y, _max.z);

    size = glm::vec3((glm::abs(_max.x) + glm::abs(_min.x)) / 2,  // X
                     (glm::abs(_max.y) + glm::abs(_min.y)) / 2,  // Y
                     (glm::abs(_max.z) + glm::abs(_min.z)) / 2); // Z

    center = _min + size;
}

bool AABB::pointInside(const glm::vec3& _point) {
    if (_point.x < vertices[0].x)
        return false;
    if (_point.y < vertices[0].y)
        return false;
    if (_point.z < vertices[0].z)
        return false;

    if (_point.x > vertices[7].x)
        return false;
    if (_point.y > vertices[7].y)
        return false;
    if (_point.z > vertices[7].z)
        return false;

    return true;
}

bool AABB::visible(Frustum& _frustum) { return _frustum.AABBVisible(vertices); }

float AABB::distance(Frustum& _frustum) { return _frustum.AABBDistance(vertices); }

// TODO: TESTAR!!!!!
// ref: https://www.gamedev.net/forums/topic/673361-axis-aligned-boxes-and-rotations/
void AABB::applyTransformation(const glm::mat4& transformation) {

    glm::vec3 val, min, max;
    for (short i = 0; i < 8; i++) {
        val = glm::vec3(transformation * glm::vec4(vertices[i], 1.0f));
        if (i != 0) {
            min = glm::min(min, val);
            max = glm::max(max, val);
        } else {
            min = val;
            max = val;
        }
    }

    this->set(min, max);
}

void AABB::render() {
    glBegin(GL_LINES);

    glVertex3fv(glm::value_ptr(vertices[0]));
    glVertex3fv(glm::value_ptr(vertices[1]));
    glVertex3fv(glm::value_ptr(vertices[2]));
    glVertex3fv(glm::value_ptr(vertices[3]));
    glVertex3fv(glm::value_ptr(vertices[4]));
    glVertex3fv(glm::value_ptr(vertices[5]));
    glVertex3fv(glm::value_ptr(vertices[6]));
    glVertex3fv(glm::value_ptr(vertices[7]));

    glVertex3fv(glm::value_ptr(vertices[0]));
    glVertex3fv(glm::value_ptr(vertices[2]));
    glVertex3fv(glm::value_ptr(vertices[1]));
    glVertex3fv(glm::value_ptr(vertices[3]));
    glVertex3fv(glm::value_ptr(vertices[4]));
    glVertex3fv(glm::value_ptr(vertices[6]));
    glVertex3fv(glm::value_ptr(vertices[5]));
    glVertex3fv(glm::value_ptr(vertices[7]));

    glVertex3fv(glm::value_ptr(vertices[0]));
    glVertex3fv(glm::value_ptr(vertices[4]));
    glVertex3fv(glm::value_ptr(vertices[1]));
    glVertex3fv(glm::value_ptr(vertices[5]));
    glVertex3fv(glm::value_ptr(vertices[2]));
    glVertex3fv(glm::value_ptr(vertices[6]));
    glVertex3fv(glm::value_ptr(vertices[3]));
    glVertex3fv(glm::value_ptr(vertices[7]));

    glEnd();
}

// void transformBox(glm::mat3x3& matrix, glm::vec3& translation, AABB& source, AABB& destination) {
//     // matrix Transform matrix.
//     // translation Translation matrix.
//     // source The original bounding box.
//     // destination The transformed bounding box.
//     // remmember to fix matrix[col][row]

//     float a, b;
//     int i, j;

//     /*Copy box A into a min array and a max array for easy reference.*/

//     glm::vec3 Amin = source.vertices[0];
//     glm::vec3 Amax = source.vertices[7];

//     /* Take care of translation by beginning at T. */
//     glm::vec3 Bmin = translation;
//     glm::vec3 Bmax = translation;

//     /* Now find the extreme points by considering the product of the */
//     /* min and max with each component of matrix.  */

//     for (i = 0; i < 3; i++)
//         for (j = 0; j < 3; j++) {
//             a = (float)(matrix[i][j] * Amin[j]);
//             b = (float)(matrix[i][j] * Amax[j]);
//             if (a < b)

//             {
//                 Bmin[i] += a;
//                 Bmax[i] += b;
//             } else {
//                 Bmin[i] += b;
//                 Bmax[i] += a;
//             }
//         }

//     /* Copy the result into the new box. */

//     destination.set(glm::vec3(Bmin[0], Bmin[1], Bmin[2]), glm::vec3(Bmax[0], Bmax[1], Bmax[2]));
// }
} // namespace Chimera