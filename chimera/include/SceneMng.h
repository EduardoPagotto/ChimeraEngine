#ifndef SCENE_MNG_H_
#define SCENE_MNG_H_

#include <vector>
#include "Node.h"
#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include "Engine3D.h"

namespace Chimera {

/**
 * Class SceneMng
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class SceneMng {
public:
    //friend class Loader;
    SceneMng(Engine3D *_pEngine3D);
    virtual ~SceneMng ();

    void addChildToScene ( Node *_pNode );

    Node *getRoot() const {
        return m_pRoot;
    }
    
    Node *getNode(EntityType _type, int index);
    
    void execute(DataMsg *dataMsg);
    
private:
  
    Engine3D *m_pEngine3D;
    
    Node *m_pRoot;

    std::vector<Camera*> m_vCamera;
    std::vector<Light*> m_vLight;
    std::vector<Object*> m_vObject;

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
