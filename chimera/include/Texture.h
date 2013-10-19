#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/gl.h>
#include <GL/glu.h>

#include "SDL2/SDL_image.h"
#include "ExceptionChimera.h"

namespace Chimera {

class Texture  {
public:
    
	friend class Material;
    
	Texture();
    Texture (const Texture &_texture);
	~Texture();
    
    int render();
    
    int init();
    
    void loadImage();
    
    void setPathFile(const std::string &_pathFile) {
        pathFile = _pathFile;
    }
    
private:
    unsigned int idTexturaGL;
    std::string pathFile;
	SDL_Surface *pImage;
};

}
#endif