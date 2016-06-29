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
    SceneMng ( Graph::Node *_pRoot, Device::Video *_pVideo );
    virtual ~SceneMng();

    void addChildToScene ( Graph::Node *_pNode );

    Graph::Node *getNode ( EntityKind _type, const std::string &_nome );
    Graph::Node *getNode ( EntityKind _type, unsigned index );

    void update ( Graph::DataMsg *dataMsg );

    Graph::Node* getRoot() const {
        return pRoot;
    }

    inline void cameraAtiva ( Graph::Camera *_pCam ) {
        pCameraAtiva = _pCam;
    }

    inline void objetoAtivo ( Graph::Object *_pObject ) {
        pObjeto = _pObject;
    }

    inline void skyBoxAtivo ( Graph::SkyBox *_pSkyBox ) {
       pSkyBox = _pSkyBox;
    }

    void setLight ( bool _lightOn );
    void setMaterial ( bool _materialOn );

    void draw ( HUD *_pHud );

private:
    void hudUpdate ( HUD *_pHud,int eye );
    void execLight();

    void parseEntity ( Graph::Node *_pNode );
    void addEntityToScene ( Graph::Node *_pNode );

    bool hasLight;
    bool hasMaterial;

    Graph::Node *pRoot;
    Graph::Camera *pCameraAtiva;
    Graph::Object *pObjeto;
    Graph::SkyBox *pSkyBox;

    std::vector<Graph::Camera*> m_vCamera;
    std::vector<Graph::Light*> m_vLight;
    std::vector<Graph::Object*> m_vObject;
    std::vector<Graph::ParticleEmitter*> m_vParticle;
    std::vector<Graph::SkyBox*> m_vSkyBox;

    ShadowMap shadoMap;

    Device::Video *pVideo;
};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
