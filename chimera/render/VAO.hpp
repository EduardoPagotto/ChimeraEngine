#ifndef __CHIMERA_VAO__HPP
#define __CHIMERA_VAO__HPP

namespace Chimera {

class VAO {
  public:
    VAO();
    virtual ~VAO();
    void create();
    void bind();
    void unbind();

  private:
    unsigned int vao;
};
} // namespace Chimera
#endif