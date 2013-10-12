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
    
    SceneMng(std::string _id, std::string _name);
    virtual ~SceneMng ();

    void addChildToScene ( Node *_pNode );

    void setEngine(Engine3D *_pEngine3D) {
        m_pEngine3D = _pEngine3D;
    }
    
    Node *getRoot() const {
        return m_pRoot;
    }
    
    Node *getNode(EntityType _type, int index);
    
    void update(DataMsg *dataMsg );
    
private:
  
    Engine3D *m_pEngine3D;
    
    Node *m_pRoot;

    std::vector<Camera*> m_vCamera;
    std::vector<Light*> m_vLight;
    std::vector<Object*> m_vObject;

//     std::list<Physics*> m_mPhysics;
     std::map<std::string, Draw*> m_mDraw;
     std::map<std::string, Image*> m_mImage;
     std::map<std::string, Effect*> m_mEffect;
     std::map<std::string, Material*> m_mMaterial;
//     std::list<btMaterial*> m_PMaterial;

};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
