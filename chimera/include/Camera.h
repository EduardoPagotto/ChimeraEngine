#ifndef CAMERA_H_
#define CAMERA_H_

#include <GL/gl.h>          // Header File For The OpenGL32 Library
#include <GL/glu.h>         // Header File For The GLu32 Library
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "Transform.h"

namespace Chimera {

enum class CameraType { 
    LAND_CAM, 
    AIR_CAM 
};    
    
class Rectangular {
public:
    Rectangular() : initX ( 0 ), initY ( 0 ), width ( 640 ) ,height ( 480 ) {}
    Rectangular ( int _x,int _y ,int _width ,int _height ) : initX ( _x ), initY ( _y ), width ( _width ) ,height ( _height ) {}
    Rectangular ( const Rectangular &_rec ) : initX ( _rec.initX ), initY ( _rec.initY ), width ( _rec.width ) ,height ( _rec.height ) {}

    inline float calcRatio() const {
        return ( float ) width / ( float ) height;
    }

    int initX,
        initY,
        width,
        height;
};

class Camera : public Transform {
public:
    Camera ( std::string name , CameraType _camType);

    virtual ~Camera();

    void setViewport ( const Rectangular _tela );

    void setProjectionPerspective ( float _fov, float _ratio, float _near, float _far );

    void setProjectionOrtogonal();

    void projection ( void );

    bool getProjection ( void ) const {
        return perspective;
    }

    float aspectRatio ( void ) const {
        return ratio;
    }

    void setProjection ( bool _val ) {
        perspective = _val;
    }

    void setAspectRatio ( void ) {
        ratio = tela.calcRatio();
    }

    virtual void update ( DataMsg *dataMsg );

    void exec ( void );
    void trackBall ( int _mx, int _my, int _mz );

    void reset();
    void update();

    void Pitch ( GLfloat theta );
    void Yaw ( GLfloat theta );
    void Roll ( GLfloat theta );

    void Walk ( GLfloat delta, bool Wall[4] );
    void Strafe ( GLfloat delta, bool Wall[4] );
    void Fly ( GLfloat delta, bool Wall[4] );

    void init ( void );

private:

    void initTrackBall ( void );

    CameraType cameraType;

    btVector3 along;
    btVector3 up;
    btVector3 forward;

    btVector3 target;

    bool perspective;
    float near;
    float far;
    float fov;

    Rectangular tela;

    float ratio;

    float horizontal;
    float vertical;
    float distancia;
    float distanciaMax;
    float distanciaMin;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
