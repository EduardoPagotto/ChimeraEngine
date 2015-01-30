#ifndef VIDEO_H_
#define VIDEO_H_

//TODO verificar em Linux se sera necessario
//#include <cstddef>
//#include <string>
//#include <iostream>

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#include <string>
#endif

namespace Chimera {

/**
 * Class Video
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Video  {
public:
    Video ( int _width, int _height, std::string _nome );
    virtual ~Video();

    const SDL_Rect *getPRectangle() const {
        return &rectangle;
    }

    std::string getNomeTela() const {
        return nomeTela;
    }
    
    void swapWindow();
         
	void setViewPortOrtogonal();
	void setViewPortOrtogonal(const SDL_Rect &_rectangle);
	void setViewPortPerspective(const float &_fov, const float &_near, const float &_far);
	void setViewPortPerspective(const SDL_Rect &_rectangle, const float &_fov, const float &_near, const float &_far);
	void initOpenGL();
	void begin2D();
	void end2D();
	void setLight ( bool _lightOn );
	void setMaterial ( bool _materialOn );

	std::string getVersaoOpenGL();

	void initScene();

private:
    std::string nomeTela;
    SDL_Rect rectangle;
    SDL_Window *window;
    SDL_GLContext context;

	bool hasLight;
	bool hasMaterial;
};

} /* namespace Chimera */
#endif /* VIDEO_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
