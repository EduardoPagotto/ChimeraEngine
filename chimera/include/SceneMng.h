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
//     std::list<btMaterial*> m_PMaterial;

};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
