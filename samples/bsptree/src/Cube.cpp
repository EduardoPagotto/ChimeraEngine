#include "Cube.hpp"

// Cube::Cube(const glm::ivec3& pos, const glm::ivec3& size, const float& sizeBlock) {
Cube::Cube(const SPACE& space, const glm::ivec3& pos, const glm::vec3& min, const glm::vec3& max) {
    this->space = space;
    this->position = pos;
    this->min = min;
    this->max = max;

    this->p[0] = glm::vec3(min.x, min.y, min.z);
    this->p[1] = glm::vec3(max.x, min.y, min.z);
    this->p[2] = glm::vec3(max.x, max.y, min.z);
    this->p[3] = glm::vec3(min.x, max.y, min.z);
    this->p[4] = glm::vec3(min.x, min.y, max.z);
    this->p[5] = glm::vec3(max.x, min.y, max.z);
    this->p[6] = glm::vec3(max.x, max.y, max.z);
    this->p[7] = glm::vec3(min.x, max.y, max.z);

    this->t[0] = glm::vec2(0, 0);
    this->t[1] = glm::vec2(1, 0);
    this->t[2] = glm::vec2(1, 1);
    this->t[3] = glm::vec2(0, 1);

    // f0 N
    this->faces.push_back(0);
    this->faces.push_back(1);
    this->faces.push_back(2);
    // f1 N
    this->faces.push_back(2);
    this->faces.push_back(3);
    this->faces.push_back(0);

    // f2 E
    this->faces.push_back(1);
    this->faces.push_back(5);
    this->faces.push_back(6);
    // f3 E
    this->faces.push_back(6);
    this->faces.push_back(2);
    this->faces.push_back(1);

    // f4 S
    this->faces.push_back(4);
    this->faces.push_back(5);
    this->faces.push_back(6);
    // f5 S
    this->faces.push_back(6);
    this->faces.push_back(7);
    this->faces.push_back(4);

    // f6 W
    this->faces.push_back(0);
    this->faces.push_back(4);
    this->faces.push_back(7);
    // f7 W
    this->faces.push_back(7);
    this->faces.push_back(3);
    this->faces.push_back(0);

    // f8 UP
    this->faces.push_back(3);
    this->faces.push_back(2);
    this->faces.push_back(6);
    // f9 UP
    this->faces.push_back(6);
    this->faces.push_back(7);
    this->faces.push_back(3);

    // f10 DOWN
    this->faces.push_back(0);
    this->faces.push_back(1);
    this->faces.push_back(5);
    // f11 DOWN
    this->faces.push_back(5);
    this->faces.push_back(4);
    this->faces.push_back(0);

    // f12 dia1
    this->faces.push_back(0);
    this->faces.push_back(5);
    this->faces.push_back(6);
    // f13 dia1
    this->faces.push_back(6);
    this->faces.push_back(3);
    this->faces.push_back(0);

    // f14 dia2
    this->faces.push_back(4);
    this->faces.push_back(1);
    this->faces.push_back(2);
    // f15 dia2
    this->faces.push_back(2);
    this->faces.push_back(7);
    this->faces.push_back(4);

    // f16 rpNS
    this->faces.push_back(0);
    this->faces.push_back(1);
    this->faces.push_back(6);
    // f17 rpNS
    this->faces.push_back(6);
    this->faces.push_back(7);
    this->faces.push_back(0);

    // f18 rpEW
    this->faces.push_back(0);
    this->faces.push_back(2);
    this->faces.push_back(6);
    // f19 rpEW
    this->faces.push_back(6);
    this->faces.push_back(4);
    this->faces.push_back(00);

    // f20 rpSN
    this->faces.push_back(5);
    this->faces.push_back(4);
    this->faces.push_back(3);
    // f21 rpSN
    this->faces.push_back(3);
    this->faces.push_back(2);
    this->faces.push_back(5);

    // f22 rpWE
    this->faces.push_back(4);
    this->faces.push_back(0);
    this->faces.push_back(2);
    // f23 rpWE
    this->faces.push_back(2);
    this->faces.push_back(6);
    this->faces.push_back(4);
}

Cube::~Cube() {}

void Cube::addFace(bool clockwise, int num) {

    int base = num * 3;

    glm::vec3 a = p[faces[base]];
    glm::vec3 b = p[faces[base + 1]];
    glm::vec3 c = p[faces[base + 2]];

    if (!clockwise) {
        // N
        std::vector<Chimera::VertexData> vl;
        vl.push_back({a, glm::vec3(0.0f), t[0]}); // n p0
        vl.push_back({b, glm::vec3(0.0f), t[1]}); // n p1
        vl.push_back({c, glm::vec3(0.0f), t[2]}); // n p2

        Chimera::Triangle t1 = Chimera::Triangle(0, 1, 2, glm::vec3(0.0f));
        t1.calcNormal(vl);
        // vl.push_back({p[3], glm::vec3(0.0f), t[3]}); // n p3
        // this->makeFaceSquare(false, vl);
    } else {
        std::vector<Chimera::VertexData> vl;
        vl.push_back({c, glm::vec3(0.0f), t[1]}); // n p0
        vl.push_back({b, glm::vec3(0.0f), t[0]}); // n p1
        vl.push_back({a, glm::vec3(0.0f), t[3]}); // n p2

        Chimera::Triangle t1 = Chimera::Triangle(0, 1, 2, glm::vec3(0.0f));
        t1.calcNormal(vl);
        // vl.push_back({p[3], glm::vec3(0.0f), t[3]}); // n p3
        // this->makeFaceSquare(false, vl);
    }
}

void Cube::addWall(CARDINAL card) {

    // if (card == CARDINAL::NORTH) {
    //     // N
    //     std::vector<Chimera::VertexData> vl;
    //     vl.push_back({p[0], glm::vec3(0.0f), t[0]}); // n p0
    //     vl.push_back({p[1], glm::vec3(0.0f), t[1]}); // n p1
    //     vl.push_back({p[2], glm::vec3(0.0f), t[2]}); // n p2

    //     Chimera::Triangle t1 = Chimera::Triangle(0, 1, 2, glm::vec3(0.0f));
    //     t1.calcNormal(vl);
    //     // vl.push_back({p[3], glm::vec3(0.0f), t[3]}); // n p3
    //     // this->makeFaceSquare(false, vl);
    // } else if (card == CARDINAL::SOUTH) {
    //     // s
    //     std::vector<Chimera::VertexData> vl;
    //     vl.push_back({p[2], glm::vec3(0.0f), t[1]}); // n p0
    //     vl.push_back({p[1], glm::vec3(0.0f), t[0]}); // n p1
    //     vl.push_back({p[0], glm::vec3(0.0f), t[3]}); // n p2

    //     Chimera::Triangle t1 = Chimera::Triangle(0, 1, 2, glm::vec3(0.0f));
    //     t1.calcNormal(vl);
    //     // vl.push_back({p[3], glm::vec3(0.0f), t[3]}); // n p3
    //     // this->makeFaceSquare(false, vl);
    // }
}