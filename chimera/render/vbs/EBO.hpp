#ifndef __CHIMERA_EBO__HPP
#define __CHIMERA_EBO__HPP

#include <vector>

namespace Chimera {

class EBO {
  public:
    EBO();
    virtual ~EBO();
    void create(std::vector<unsigned int>& index);
    void render();

  private:
    unsigned int eboGL;
    unsigned int sizeIndex;
};
} // namespace Chimera
#endif