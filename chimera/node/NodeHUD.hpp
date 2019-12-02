#ifndef __CHIMERA__HUD__HPP
#define __CHIMERA__HUD__HPP

#include <vector>

#include "Font.hpp"
#include "Node.hpp"
#include "chimera/core/Shader.hpp"

namespace Chimera {
class NodeHUD : public Node {
  public:
    struct HUDTxt {
        int indexFonte;
        int posX;
        int posY;
        std::string* pText;
        GLfloat scale;
        glm::vec4 color;
        // Color color;
    };

    NodeHUD(Node* _parent, std::string _name);

    virtual ~NodeHUD();

    virtual void init() override;
    virtual void accept(class VisitorInterface* v) override;

    void render(Shader* _pShader);

    void addFont(Font* _pFont);

    inline void setOn(const bool _on) { on = _on; }

    inline bool isOn(void) const { return on; }

    void addText(int _fontIndex, int _posX, int _posY, glm::vec4 _colorText, GLfloat _scale, std::string* _txt);

  private:
    bool on;
    std::vector<HUDTxt*> vLineText;
    std::vector<Font*> vFonts;
};
} // namespace Chimera
#endif
