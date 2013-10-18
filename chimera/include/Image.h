#ifndef IMAGE_H_
#define IMAGE_H_

#include "SDL2/SDL_image.h"
#include "ExceptionChimera.h"
#include "Node.h"


namespace Chimera {

class Image : public Node {
public:
	friend class Material;
	Image();
    Image (const Image &_imagen);
	~Image();
    
    virtual void update ( DataMsg *dataMsg );
    
    void loadImage();
    
    void setPathFile(const std::string &_pathFile) {
        pathFile = _pathFile;
    }
    
private:
    std::string pathFile;
	SDL_Surface *pImage;
};

}
#endif