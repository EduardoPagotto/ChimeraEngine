#ifndef SCENE_MNG_H_
#define SCENE_MNG_H_

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "ExceptionSDL.h"
#include "Video.h"
#include "Node.h"
#include "Camera.h"
#include "Object.h"
#include "Light.h"

namespace Chimera {

/**
 * Class SceneMng
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class SceneMng {
public:
    //friend class Loader;
    SceneMng(Video *_video);
    virtual ~SceneMng ();

    void addChildToScene ( Node *_pNode );
    void setLight ( bool _lightOn );
    void setMaterial ( bool _materialOn );
    void begin2D();
    void end2D();

    void open();
    void close();

    //void run();
    
//     void draw() {
//         //FIXME Aqui tb!!!!
//         //parse(true,Chimera::DO_DRAW_3D,NULL,NULL);
//     }

    Node *getRoot() const {
        return m_pRoot;
    }

private:
    bool m_quit;
    bool m_hasLight;
    bool m_hasMaterial;

    Node *m_pRoot;

    std::list<Camera*> m_lCamera;
    std::list<Light*> m_lLight;
    std::list<Object*> m_lObject;

    Video *m_pVideo;
//     std::list<Physics*> m_mPhysics;
//     std::list<Draw*> m_mDraw;
//     std::list<Image*> m_mImage;
//     std::list<Effect*> m_mEffect;
//     std::list<Material*> m_mMaterial;
//     std::list<btMaterial*> m_PMaterial;

};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
