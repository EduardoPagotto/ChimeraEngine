#ifndef SCENE_MNG_H_
#define SCENE_MNG_H_

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
    SceneMng();
    virtual ~SceneMng ();

    void addChildToScene ( Node *_pNode );

    Node *getRoot() const {
        return m_pRoot;
    }
    
private:
  
    Node *m_pRoot;

    std::list<Camera*> m_lCamera;
    std::list<Light*> m_lLight;
    std::list<Object*> m_lObject;

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
