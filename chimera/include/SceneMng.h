#ifndef SCENE_MNG_H_
#define SCENE_MNG_H_

#include <map>
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
class SceneMng : public Node {
public
:
    friend class Loader;
    
    SceneMng();
    virtual ~SceneMng ();

    void addChildToScene ( Node *_pNode );
        
    Node *getNode(EntityKind _type, int index);
    
    void update(DataMsg *dataMsg );
    
    void execLight();
    
private: 
    std::vector<Camera*> m_vCamera;
    std::vector<Light*> m_vLight;
    std::vector<Object*> m_vObject;

//     std::list<Physics*> m_mPhysics;
     std::map<std::string, Draw*> m_mDraw;
//     std::map<std::string, Image*> m_mImage;
//     std::map<std::string, Effect*> m_mEffect;
//     std::map<std::string, Material*> m_mMaterial;
//     std::list<btMaterial*> m_PMaterial;

};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
