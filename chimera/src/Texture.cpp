#include "Texture.h"
#include "Video.h"

namespace Chimera {
namespace Graph {
    
Texture::Texture ( std::string _id, std::string _name ) : Entity ( EntityKind::TEXTURE, _id, _name ) {

    texturaCarregada = false;
    pathFile = "";
    setFilter ( TextureFilter::Nearest );
    textureList[0] = 0;
    textureList[1] = 0;
    textureList[2] = 0;

}

Texture::Texture ( const Texture &_texture ) : Entity ( _texture ) {

    pathFile = _texture.pathFile;
    indiceFilter = _texture.indiceFilter;
    texturaCarregada = _texture.texturaCarregada;
    textureList[0] = _texture.textureList[0];
    textureList[1] = _texture.textureList[1];
    textureList[2] = _texture.textureList[2];

}

Texture::~Texture() {

    glDeleteTextures ( 3, ( GLuint* ) &textureList[0] );

}

void Texture::setFilter ( TextureFilter _filter ) {

    switch ( _filter ) {
    case TextureFilter::Nearest:
        indiceFilter = 0;
        break;
    case TextureFilter::Linear:
        indiceFilter = 1;
        break;
    case TextureFilter::Mipmapped:
        indiceFilter = 2;
        break;

    }

}

TextureFilter Texture::getFilter() {

    TextureFilter retorno;

    switch ( indiceFilter ) {
    case 0:
        retorno = TextureFilter::Nearest;
        break;
    case 1:
        retorno = TextureFilter::Linear;
        break;
    case 2:
        retorno = TextureFilter::Mipmapped;
        break;

    }

    return retorno;

}


void Texture::end() {
    glBindTexture ( GL_TEXTURE_2D, 0 );
}


void Texture::begin() {

    glBindTexture ( GL_TEXTURE_2D, textureList[indiceFilter] );

}

void Texture::init() {

    if ( texturaCarregada == false ) {

        SDL_Surface *pImage = loadImage();

        // Create The Texture
        glGenTextures ( 3, ( GLuint* ) &textureList[0] );

        // Load in texture 1
        glBindTexture ( GL_TEXTURE_2D, textureList[0] ); // Typical Texture Generation Using Data From The Bitmap

        // Generate The Texture
        if ( pImage->format->Amask != 0 ) {
            glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, pImage->w, pImage->h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pImage->pixels );
        } else {
            glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, pImage->w, pImage->h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pImage->pixels );
        }

        // Nearest Filtering
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

        // Load in texture 2
        glBindTexture ( GL_TEXTURE_2D, textureList[1] ); // Typical Texture Generation Using Data From The Bitmap

        // Linear Filtering
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        // Generate The Texture
        if ( pImage->format->Amask != 0 ) {
            glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, pImage->w, pImage->h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pImage->pixels );
        } else {
            glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, pImage->w, pImage->h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pImage->pixels );
        }

        // Load in texture 3
        glBindTexture ( GL_TEXTURE_2D, textureList[2] ); // Typical Texture Generation Using Data From The Bitmap

        // Mipmapped Filtering
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        // Generate The MipMapped Texture ( NEW )
        if ( pImage->format->Amask != 0 ) {
            gluBuild2DMipmaps ( GL_TEXTURE_2D, 4, pImage->w, pImage->h, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pImage->pixels );
        } else {
            gluBuild2DMipmaps ( GL_TEXTURE_2D, 3, pImage->w, pImage->h, GL_BGR_EXT, GL_UNSIGNED_BYTE, pImage->pixels );
        }

        SDL_FreeSurface ( pImage );

        texturaCarregada = true;
    }

}

SDL_Surface *Texture::loadImage() {

    SDL_Surface *pImage = IMG_Load ( pathFile.c_str() );
    if ( pImage != nullptr ) {
        return pImage;
    }

    throw ExceptionChimera ( ExceptionCode::READ, "Falha ao ler arquivo:" + pathFile );
}

void Texture::clone ( Entity **ppNode ) {
    *ppNode = new Texture ( *this );
}

void Texture::createRenderTexture ( int sizeX, int sizeY, int channels, int type ) {

    // This function is a modified version of what we have used in previous tutorials.
    // We just allowed the option of doing a depth texture.  When doing a depth
    // texture we need to pass in GL_DEPTH_COMPONENT for the type AND the channels.
    // To check for this we just see if the channels are greater than 4.  If so, we
    // allocate a texture for just one bit per pixel, then pass in the channel value
    // for glTexImage2D().

    // Create a pointer to store the blank image data
    unsigned int *pTexture = NULL;

    // Store the current channels to be allocated by default
    int channelsTrue = channels;

    // If the channels are greater than 4 there must be a special flag like
    // GL_DEPTH_COMPONENT, so make sure we only allocate 1 bit per pixel.
    if ( channels > 4 ) {
        channelsTrue = 1;
    }

    // Allocate and init memory for the image array and point to it from pTexture
    pTexture = new unsigned int[sizeX * sizeY * channelsTrue];
    memset ( pTexture, 0, sizeX * sizeY * channelsTrue * sizeof ( unsigned int ) );

    // Register the texture with OpenGL and bind it to the texture ID
    glGenTextures ( 1, ( GLuint* ) &textureList[0] );
    //glGenTextures(1, &textureArray[textureID]);
    glBindTexture ( GL_TEXTURE_2D, textureList[0] );
    //glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

    // Create the texture and store it on the video card
    glTexImage2D ( GL_TEXTURE_2D, 0, channels, sizeX, sizeY, 0, type, GL_UNSIGNED_INT, pTexture );
    //TODO avalisar glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage->w, pImage->h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pImage->pixels);

    // Set the texture quality
    glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    // Since we stored the texture space with OpenGL, we can delete the image data
    delete[] pTexture;
}

void Texture::beginTexCreated() {

    // Turn on our texture unit for shadow mapping and bind our depth texture
    glActiveTextureARB ( GL_TEXTURE0_ARB );
    glEnable ( GL_TEXTURE_2D );
    begin();
    //glBindTexture(GL_TEXTURE_2D, g_Texture[SHADOW_ID]);

    // Here is where we set the mode and function for shadow-mapping in fixed functionality.
    // The mode we use is GL_TEXTURE_COMPARE_MODE_ARB, with GL_COMPARE_R_TO_TEXTURE_ARB, which
    // tells OpenGL that we want to compare the depth value in our world to the current
    // texture's depth value.  We then set the compare function to GL_LEQUAL, which says that
    // we will not shadow the current pixel if the depth value is less than or equal to
    // the texture's depth value.
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL );

    // Now comes the complicated part to grasp at first.  If you have never used the
    // texture generation functionality in OpenGL for sphere mapping or something else,
    // I will give a basic explanation of what it does.  So that we don't have to
    // do the work ourselves of calculating every UV coordinate, we can have the UV
    // coordinates generated by OpenGL for us, given a bunch of different inputs to work from.
    // We can use OpenGL's texture generation functionality to project our camera depth values
    // into the light's clip space.  Since our matrices are 4x4, we need to do our
    // projection in 4D.  Thus, we use texture coordinates S, T, R and Q.

    // We first create planes for each axis (x, y, z, w).  This will be used
    // with our projection of our camera's view to the light's clip space and
    // make sure the texture coordinates don't move when our camera moves, but
    // are the same no matter where our camera is looking.  The equation for
    // doing projected texturing is:
    //
    // mProjectedTexture = mLightProjection * mLightModelview * mCameraInverse;
    //
    // We say the "camera inverse", but really it is the modelview matrix's inverse
    // that has the camera's view applied to it.  So this is what we mean when we say that.
    // Usually we will need to multiply the inverse of our camera matrix by the projection
    // and modelview matrix of the light; however, when using these planes for our camera
    // OpenGL will calculate the inverse of our camera for us.  In the GLSL version of
    // shadow mapping we calculate the camera inverse ourselves.

    // Create the eye planes for generating texture coordinates
    const double xPlane[] = { 1.0, 0.0, 0.0, 0.0 };
    const double yPlane[] = { 0.0, 1.0, 0.0, 0.0 };
    const double zPlane[] = { 0.0, 0.0, 1.0, 0.0 };
    const double wPlane[] = { 0.0, 0.0, 0.0, 1.0 };

    // Enable texture generation for S, T, R, and Q
    glEnable ( GL_TEXTURE_GEN_S );
    glEnable ( GL_TEXTURE_GEN_T );
    glEnable ( GL_TEXTURE_GEN_R );
    glEnable ( GL_TEXTURE_GEN_Q );

    // Set each texture coordinate's texture gen mode to use the camera.
    // This will have OpenGL incorporate the camera's inverse for us.
    glTexGeni ( GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
    glTexGeni ( GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
    glTexGeni ( GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
    glTexGeni ( GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );

    // Set our planes for each coordinate in order to project the texture appropriatly.
    glTexGendv ( GL_S, GL_EYE_PLANE, xPlane );
    glTexGendv ( GL_T, GL_EYE_PLANE, yPlane );
    glTexGendv ( GL_R, GL_EYE_PLANE, zPlane );
    glTexGendv ( GL_Q, GL_EYE_PLANE, wPlane );

}

void Texture::endTexCreated() {

    // Turn off texture generation for our S, T, R and Q coordinates
    glDisable ( GL_TEXTURE_GEN_S );
    glDisable ( GL_TEXTURE_GEN_T );
    glDisable ( GL_TEXTURE_GEN_R );
    glDisable ( GL_TEXTURE_GEN_Q );

    end();

    // Turn the first multi-texture pass off
    glActiveTextureARB ( GL_TEXTURE0_ARB );
    glDisable ( GL_TEXTURE_2D );

}
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
