#ifndef IMAGE_H_
#define IMAGE_H_

#include <GL/gl.h>
#include <GL/glu.h>

#include "SDL2/SDL_image.h"
#include "ExceptionChimera.h"
#include "Node.h"

namespace Chimera {

class Texture : public Node {
public:
	friend class Material;
	Texture();
    Texture (const Texture &_texture);
	~Texture();
    
    int link();
    
    virtual void update ( DataMsg *dataMsg );
    
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