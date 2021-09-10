#ifndef __CHIMERA_VAO__HPP
#define __CHIMERA_VAO__HPP

#include "Buffer.hpp"

namespace Chimera {

class VAO {
  public:
    VAO();
    virtual ~VAO();
    void addBuffer(VBO* buffer, unsigned int index);
    inline void bind() const { glBindVertexArray(arrayID); }
    inline void unbind() const { glBindVertexArray(0); }

  private:
    unsigned int arrayID;
    std::vector<VBO*> buffers;
};
} // namespace Chimera
#endif