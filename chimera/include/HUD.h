#ifndef HUD_H_
#define HUD_H_

#include "Draw.h"

#include <vector>

// #ifndef WIN32
// #include <SDL2/SDL.h>
// #else
// #include <SDL.h>
// #endif

#include "Font.h"

//#include <glm/glm.hpp>

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
    //void addSquare ( const SDL_Rect _rec, Color _color );

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
    //void drawBoxes();
    void drawFonts();

    bool on;
    //std::vector<HUDSquare*> vSquare;
    std::vector<HUDTxt*> vLineText;
    std::vector<Font*> vFonts;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
