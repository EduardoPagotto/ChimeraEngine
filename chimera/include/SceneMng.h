#ifndef SCENE_MNG_H_
#define SCENE_MNG_H_

#include <vector>

#include "Video.h"

#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include "ParticleEmitter.h"
#include "SkyBox.h"
#include "HUD.h"

#include "ShadowMap.h"

namespace Chimera
{

/**
 * Class SceneMng
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class SceneMng
{
public:
    SceneMng ( Node *_pRoot, Video *_pVideo );
    virtual ~SceneMng();

    void addChildToScene ( Node *_pNode );

    Node *getNode ( EntityKind _type, const std::string &_nome );
    Node *getNode ( EntityKind _type, unsigned index );

    void update ( DataMsg *dataMsg );

    Node* getRoot() const {
        return pRoot;
    }

    inline void cameraAtiva ( Camera *_pCam ) {
        pCameraAtiva = _pCam;
    }

    inline void objetoAtivo ( Object *_pObject ) {
        pObjeto = _pObject;
    }

    inline void skyBoxAtivo ( SkyBox *_pSkyBox ) {
       pSkyBox = _pSkyBox;
    }

    void setLight ( bool _lightOn );
    void setMaterial ( bool _materialOn );

    void draw ( HUD *_pHud );

private:
    void hudUpdate ( HUD *_pHud,int eye );
    void execLight();

    void parseEntity ( Node *_pNode );
    void addEntityToScene ( Node *_pNode );

    bool hasLight;
    bool hasMaterial;

    Node *pRoot;
    Camera *pCameraAtiva;
    Object *pObjeto;
    SkyBox *pSkyBox;

    std::vector<Camera*> m_vCamera;
    std::vector<Light*> m_vLight;
    std::vector<Object*> m_vObject;
    std::vector<ParticleEmitter*> m_vParticle;
    std::vector<SkyBox*> m_vSkyBox;

    ShadowMap shadoMap;

    Video *pVideo;
};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
