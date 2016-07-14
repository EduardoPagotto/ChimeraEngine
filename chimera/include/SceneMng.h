#ifndef SCENE_MNG_H_
#define SCENE_MNG_H_

#include <vector>

#include "Video.h"
#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include "ParticleEmitter.h"
#include "HUD.h"

#include "ShadowMap.h"
#include "SceneRoot.h"
#include "LoaderDae.h"

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
    SceneMng ( Video *_pVideo );
    virtual ~SceneMng();

    Node *getNode ( EntityKind _type, const std::string &_nome );
    
    Node *getNode ( EntityKind _type, unsigned index );

    void init();
    
    void setReader( LoaderDae *_pLoader );
    
    Group* createSceneGraph();
    
    inline void cameraAtiva ( Camera *_pCam ) {
        pCameraAtiva = _pCam;
    }

    inline void objetoAtivo ( Object *_pObject ) {
        pObjeto = _pObject;
    }

	SceneRoot *getRoot() {
		return root;
	}

//    void setLight ( bool _lightOn );
//    void setMaterial ( bool _materialOn );

    void draw ( HUD *_pHud );

private:
    void hudUpdate ( HUD *_pHud,int eye );

    void parseEntity ( Node *_pNode );
    void addEntityToScene ( Node *_pNode );

    LoaderDae *pLoader;
    
    SceneRoot *root;
    Camera *pCameraAtiva;
    Object *pObjeto;

    std::vector<Camera*> m_vCamera;
    std::vector<Light*> m_vLight;
    std::vector<Object*> m_vObject;

    ShadowMap shadoMap;

    Video *pVideo;
};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
