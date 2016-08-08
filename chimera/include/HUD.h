#ifndef HUD_H_
#define HUD_H_

#include "Draw.h"

#include <vector>

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include "Font.h"

namespace Chimera
{
class HUD : public Draw
{
public:

    struct HUDSquare {
        SDL_Rect tela;
        Color color;
    };

    struct HUDTxt {
        int indexFonte;
        int indexSquare;
        int posX;
        int posY;
        std::string *pText;
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
    void addSquare ( const SDL_Rect _rec, Color _color );

    inline void setOn ( const bool _on ) {
        on = _on;
    }

    inline bool isOn ( void ) const {
        return on;
    }

    void addText ( int _fontIndex, int _squareIndex, int _posX, int _posY, Color _colorText, std::string *_txt );

private:
    void drawBoxes();
    void drawFonts();

    bool on;
    std::vector<HUDSquare*> vSquare;
    std::vector<HUDTxt*> vLineText;
    std::vector<Font*> vFonts;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
