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

	// Inherited via Node
	void accept(class NodeVisitor* v) override;

	// Inherited via Draw
	virtual void render(const glm::mat4 &_projection, const glm::mat4 &_view, const glm::mat4 &_model) override;
	virtual glm::vec3 getSizeBox() override;

    void addFont ( Font *_pFont );

    inline void setOn ( const bool _on ) {
        on = _on;
    }

    inline bool isOn ( void ) const {
        return on;
    }

    void addText ( int _fontIndex, int _posX, int _posY, Color _colorText, GLfloat _scale, std::string *_txt );

private:
    bool on;
    std::vector<HUDTxt*> vLineText;
    std::vector<Font*> vFonts;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
