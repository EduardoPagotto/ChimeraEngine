#ifndef TEXTURE_H_
#define TEXTURE_H_

#ifdef WIN32
#include "SDL_image.h"
#else
#include "SDL2/SDL_image.h"
#endif
#include "ExceptionChimera.h"

#include "Entity.h"

namespace Chimera {
namespace Graph {
    
enum class TextureFilter
{
    Nearest,
    Linear,
    Mipmapped
};


class Texture : public Entity
{
public:
    Texture ( std::string _name, std::string _pathFile );
    Texture ( const Texture &_texture );

    virtual ~Texture();

    virtual void clone ( Entity **ppNode );

    void begin();
    void end();
    void init();

    SDL_Surface *loadImage();

    void setFilter ( TextureFilter _filter );

    TextureFilter getFilter();

    // novo usado em ShadowMap
    // This function creates a blank texture to render to
    void createRenderTexture ( int sizeX, int sizeY, int channels, int type );
    void beginTexCreated();
    void endTexCreated();

private:
    std::string pathFile;

    int textureList[3];
    int indiceFilter;
    bool texturaCarregada;
};
}
}
#endif
