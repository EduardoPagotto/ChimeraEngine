#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/gl.h>
#include <GL/glu.h>

#include <log4cxx/logger.h>

#include "SDL2/SDL_image.h"
#include "ExceptionChimera.h"

#include "Node.h"

namespace Chimera {

enum class TextureFilter {
    Nearest,
    Linear,
    Mipmapped
};
    
    
class Texture : public Node {
public:
        
    Texture(std::string _id, std::string _name);
    Texture (const Texture &_texture);
    
	virtual ~Texture();
    
    virtual void update ( DataMsg *dataMsg );
    
    void render(); 
    void init();
    
    SDL_Surface *loadImage();
    
    void setPathFile(const std::string &_pathFile) {
        pathFile = _pathFile;
    }
    
    void setFilter(TextureFilter _filter);
    
    TextureFilter getFilter();
    
private:
    std::string pathFile;
    GLuint textureList[3];
    int indiceFilter;
    bool texturaCarregada;
    
    log4cxx::LoggerPtr logger;
};

}
#endif