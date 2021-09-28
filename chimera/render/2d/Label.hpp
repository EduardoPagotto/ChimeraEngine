#ifndef __CHIMERA_LABEL_2D__HPP
#define __CHIMERA_LABEL_2D__HPP

#include "Renderable2D.hpp"
#include "chimera/render/FontAtlas.hpp"

namespace Chimera {

class Label : public Renderable2D {

  public:
    Label(const std::string& text, float x, float y, const std::string& font, const glm::vec4& color);
    virtual ~Label();
    virtual void submit(IRenderer2D* renderer);
    inline void setText(const std::string& text) { this->text = text; }

  private:
    std::string text;
    FontAtlas* font;
};

} // namespace Chimera

#endif