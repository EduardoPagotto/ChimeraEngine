#ifndef __CHIMERA_EBO__HPP
#define __CHIMERA_EBO__HPP

#include "chimera/OpenGLDefs.hpp"
#include <vector>

namespace Chimera {

class EBO {
  public:
    EBO(std::vector<uint32_t>& index);
    virtual ~EBO();
    void render();
    inline void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID); }
    inline void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

  private:
    unsigned int bufferID;
    unsigned int sizeIndex;
};
} // namespace Chimera
#endif