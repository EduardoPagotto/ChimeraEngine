#ifndef ENGINE_3D_H_
#define ENGINE_3D_H_

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "ExceptionSDL.h"
#include "Timer.h"
#include "Video.h"
#include "Camera.h"

namespace Chimera {
    
class Engine3D {
public:
    Engine3D();
    virtual ~Engine3D();
        
    void initOpenGL(Video *_pVideo);
    
    std::string getVersaoOpenGL();
    
    //void setLight ( bool _lightOn );
    //void setMaterial ( bool _materialOn );
    //void begin2D();
    //void end2D();
        
    void setViewPortOrtogonal();
    void setViewPortOrtogonal(int _posInitX, int _posIntY, int _width, int _height);
    
    void setViewPortPerspective(Camera *_camera);
    void setViewPortPerspective(float _near, float _far, float _fov);
    void setViewPortPerspective(int _posInitX, int _posIntY, int _width, int _height, float _near, float _far, float _fov);

//     bool getLight() {
//         return hasLight;
//     }
//     
//     bool getMateria() {
//         return hasMaterial;
//     }
    
    Video *getVideo() {
        return pVideo;
    }
    
private:
    
    bool hasLight;
    bool hasMaterial;
    Video *pVideo;
    
};

}

#endif