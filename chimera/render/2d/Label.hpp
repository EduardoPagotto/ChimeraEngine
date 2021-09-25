#ifndef __CHIMERA_LABEL_2D__HPP
#define __CHIMERA_LABEL_2D__HPP

#include "Renderable2D.hpp"

namespace Chimera {

class Label : public Renderable2D {

  public:
    Label(const std::string& text, float x, float y, const glm::vec4& color);
    virtual ~Label();

    virtual void submit(IRenderer2D* renderer);

  private:
    std::string text;
};

} // namespace Chimera

#endif