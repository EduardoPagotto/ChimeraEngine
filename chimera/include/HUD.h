#ifndef HUD_H_
#define HUD_H_

#include <vector>

#include "Draw.h"
#include "Font.h"

namespace Chimera
{
class HUD : public Draw
{
public:

    struct HUDTxt {
        int indexFonte;
        int posX;
        int posY;
        std::string *pText;
        GLfloat scale;
        Color color;
    };

    HUD(Node *_parent, std::string _name);

    virtual ~HUD();

	virtual void init();
	void accept(class NodeVisitor* v);

	// Inherited via Draw
	virtual void renderExecute(bool _texture) override;
	virtual glm::vec3 getSizeBox() override;

    void addFont ( Font *_pFont );

    inline void setOn ( const bool _on ) {
        on = _on;
    }

    inline bool isOn ( void ) const {
        return on;
    }

    void addText ( int _fontIndex, int _posX, int _posY, Color _colorText, GLfloat _scale, std::string *_txt );

    Shader shade;
    
    glm::mat4 projection;
    
private:
    void drawFonts();

    bool on;
    std::vector<HUDTxt*> vLineText;
    std::vector<Font*> vFonts;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
