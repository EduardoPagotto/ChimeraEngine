#ifndef __CHIMERA_NODE_HUD__HPP
#define __CHIMERA_NODE_HUD__HPP

#include <vector>

#include "Node.hpp"
#include "chimera/render/Font.hpp"
#include "chimera/render/Shader.hpp"

namespace Chimera {
class NodeHUD : public Node {
  public:
    struct HUDTxt {
        int indexFonte;
        glm::ivec2 pos;
        std::string* pText;
        GLfloat scale;
        glm::vec4 color;
    };

    NodeHUD(Node* _parent, std::string _name);

    virtual ~NodeHUD();

    virtual void init() override;
    virtual void accept(class VisitorInterface* v) override;

    void render(Shader* _pShader);

    void addFont(Font* _pFont);

    inline void setOn(const bool _on) { on = _on; }

    inline bool isOn(void) const { return on; }

    void addText(int _fontIndex, const glm::ivec2& _pos, glm::vec4 _colorText, GLfloat _scale, std::string* _txt);

  private:
    bool on;
    std::vector<HUDTxt*> vLineText;
    std::vector<Font*> vFonts;
};
} // namespace Chimera
#endif
