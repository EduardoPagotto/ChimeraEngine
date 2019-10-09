#ifndef __CHIMERA_LOAD_OBJ__HPP
#define __CHIMERA_LOAD_OBJ__HPP

namespace chimera {

struct Meshtmp {

    std::vector<unsigned int> vertexIndex;
    std::vector<glm::vec3> vertexList;

    std::vector<unsigned int> normalIndex;
    std::vector<glm::vec3> normalList;

    std::vector<unsigned int> textureIndex;
    std::vector<glm::vec2> textureList;

};

} // namespace chimera

#endif