#ifndef VIDEO_H_
#define VIDEO_H_

//#include <GL/glew.h>
//#include <GL/glfw3native.h>
//#include <GL/glfw3.h>
//#include <glm/glm.hpp>
/*
 * http://www.opengl-tutorial.org/
 * http://www.opengl-tutorial.org/miscellaneous/useful-tools-links/
 * http://www.opengl-tutorial.org/miscellaneous/building-your-own-c-application/
 *
 *
 * https://github.com/meandmark/SDLOpenGLIntro/blob/sdl2/GameApp.cpp
 * http://meandmark.com/blog/2013/06/creating-an-opengl-context-in-sdl2/
 *
 * verificar GLFW, GLM, GLEW
 */

#include <cstddef>
#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "ExceptionSDL.h"
#include "Timer.h"

namespace Chimera {

/**
 * Class Timer
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Video  {
public:
    Video ( int width, int height, std::string nome );
    virtual ~Video();

    inline int getWidth() const {
        return width;
    }
    inline int getHeight() const {
        return height;
    }

    void open();
    void close();
    void execute ();
    void swapWindow();
    
private:

    void renderize();
    void initGL();

    int width;
    int height;
    std::string nomeTela;

    SDL_Window *window;
    SDL_GLContext context;

    Timer timerFPS;
};

} /* namespace Chimera */
#endif /* VIDEO_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
