#ifndef SCENE_MNG_H_
#define SCENE_MNG_H_

#include <vector>
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
    SceneMng(Node *_pRoot);
    virtual ~SceneMng ();

    void addChildToScene ( Node *_pNode );
        
    Node *getNode(EntityKind _type, int index);
    
    void update(DataMsg *dataMsg );
    
    void execLight();
    
    void setViewPortOrtogonal(const SDL_Rect &_rectangle);
    void setViewPortPerspective(const SDL_Rect &_rectangle, const Camera *_camera);
       

    
private:
    
    void parseEntity(Node *_pNode);
    void addEntityToScene( Node *_pNode);
    
    Node *pRoot;
    
    std::vector<Camera*> m_vCamera;
    std::vector<Light*> m_vLight;
    std::vector<Object*> m_vObject;
    std::vector<Draw*> m_vDraw;
    std::vector<Image*> m_vImage;
    std::vector<Material*> m_vMaterial;
    std::vector<Effect*> m_vEffect;
//     std::list<Physics*> m_mPhysics;
//     std::map<std::string, Draw*> m_mDraw;
//     std::map<std::string, Image*> m_mImage;
//     std::map<std::string, Effect*> m_mEffect;
//     std::map<std::string, Material*> m_mMaterial;
//     std::list<btMaterial*> m_PMaterial;

};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
