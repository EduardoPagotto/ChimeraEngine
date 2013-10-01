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
        
    void initOpenGL(Video *pVideo);
    
    std::string getVersaoOpenGL();
    
    void setLight ( bool _lightOn );
    void setMaterial ( bool _materialOn );
    void begin2D();
    void end2D();
        
    void setViewPortOrtogonal();
    void setViewPortOrtogonal(int posInitX, int posIntY, int width, int height);
    
    void setViewPortPerspective(Camera *_camera);
    void setViewPortPerspective(float near, float far, float fov);
    void setViewPortPerspective(int posInitX, int posIntY, int width, int height, float near, float far, float fov);

    bool getLight() {
        return m_hasLight;
    }
    
    bool getMateria() {
        return m_hasMaterial;
    }
    
    Video *getVideo() {
        return m_pVideo;
    }
    
private:
    
    bool m_hasLight;
    bool m_hasMaterial;
    Video *m_pVideo;
    
};

}

#endif